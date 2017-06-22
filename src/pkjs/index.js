// Import the Clay package
var Clay = require('pebble-clay');
// Import messageKeys module to log data
var messageKeys = require('message_keys');
// Load our Clay configuration file
var clayConfig = require('./config.js');
var clayConfigFR = require('./config-fr.js');
var clayConfigES = require('./config-es.js');
var clayConfigDE = require('./config-de.js');
// Custom function that runs on the config page
var customClay = require('./custom-clay.js');

// Initialize Clay
var clay = new Clay(clayConfig, customClay, { autoHandleEvents: false});
								 
// Receiving AppMessage events
Pebble.addEventListener('appmessage', function(e) {
  // Get the dictionary from the message
  var dict = e.payload;
	
	// Set JSON values to match those in the dictionary
	if ('0' in dict) {
		console.log('Received achievements from watch!');
		if (localStorage.getItem('achievements' !== null)) {
			var achievements = JSON.parse(localStorage.getItem('achievements'));
		} else {
			var achievements = new Array();
		}
		for (var key in dict) {
			achievements[key] = dict[key];
		}
		localStorage.setItem('achievements', JSON.stringify(achievements));
		
	} else if ('rememberDuration' in dict) {
		console.log('Received settings from watch!');
		// Get the clay settings object from localStorage
		var settings = JSON.parse(localStorage.getItem('clay-settings'));
		for (var key in dict) {
			if (key == 'rememberDuration' || key == 'vibrationEnabled' || key == 'appGlanceEnabled' || key == 'achievementsEnabled') {
				var booleanValue = dict[key] == 1; // Convert integer to boolean
				settings[key] = booleanValue;
			} else {
				settings[key] = dict[key];
			}
		}
		// Convert JSON back to object, then save the settings object back into localStorage
		localStorage.setItem('clay-settings', JSON.stringify(settings));
	}
});

// As soon as PebbleKit JS has been initialized, send request to get settings from watch
Pebble.addEventListener('ready', function() {
	var info = Pebble.getActiveWatchInfo();
	
	if (info.platform != 'aplite') { // Do not get settings from watch if is on Aplite watch
			requestSettings();
			setTimeout(sendAchievements, 200);
	}
});

function requestSettings() {
	if (localStorage.getItem('clay-settings') !== null) {
		var dict = {};
				dict[messageKeys.requestSettings] = 'true';

				Pebble.sendAppMessage(dict, function() {
					console.log('Settings Request sent successfully!');
				}, function(e) {
					console.log('Settings Request failed: ' + JSON.stringify(e));
		});
	}
}

var failedCount = 0;
function sendAchievements() {
	if (localStorage.getItem('achievements') !== null) {
		var achievements = JSON.parse(localStorage.getItem('achievements'));

		var dict = {};
		for (var key in achievements) {
			dict[key] = achievements[key];
		}
		
		Pebble.sendAppMessage(dict, function() {
			console.log('Achievements sent successfully!');
		}, function (e) {
			console.log('Achievement sending failed: ' + JSON.stringify(e));
			failedCount++;
			if (failedCount < 3) {
				setTimeout(sendAchievements, 1000); // Tries to send achievements again in a second.
			}
		});
	} else {
		console.log('There are no achievements to send.');
	}
}

// Open different config page for each language
Pebble.addEventListener('showConfiguration', function(e) {
	// Grab achievements and put into string for backup
	var achievementsString = "";
	if (localStorage.getItem('achievements') !== null) {
		var achievements = JSON.parse(localStorage.getItem('achievements'));
		for (var key in achievements) {
			if (key == '0' || key == '1' || key == '3') {
				achievementsString += ("00000" + achievements[key].toString()).slice(-5);
			} else if (key == '2') {
				achievementsString += ("0000000000" + achievements[key].toString()).slice(-10);
			} else {
				achievementsString += achievements[key].toString();
			}
		}
		// Set the achievementsBackup key/value in Clay to show in settings page
		var settings = JSON.parse(localStorage.getItem('clay-settings'));
		settings['achievementsBackup'] = achievementsString;
		localStorage.setItem('clay-settings', JSON.stringify(settings));
		console.log(JSON.stringify(settings));
	} else {
		console.log('No achievements are stored in localStorage.');
	}
	
	var info = Pebble.getActiveWatchInfo();
	
	// Show the configuration page
	console.log('The language of the phone is ' + info.language.substr(0, 2));
	switch (info.language.substr(0, 2)) {
		case 'fr': // The watch language is French; show the French config page
			clay.config = clayConfigFR;
			break;
		case 'es': // The watch language is Spanish; show the Spanish config page
			clay.config = clayConfigES;
			break;
		case 'de': // The watch language is German; show the German config page
			clay.config = clayConfigDE;
			break;
	}
	
	Pebble.openURL(clay.generateUrl());
});

// When config page is closed, send settings to watch
Pebble.addEventListener('webviewclosed', function(e) {
	if (e && !e.response) {
		return;
	}

	// Get the keys and values from each config item
	var dict = clay.getSettings(e.response);
	dict[messageKeys.reminderHours] = parseInt(dict[messageKeys.reminderHours]);
	dict[messageKeys.displayText] = parseInt(dict[messageKeys.displayText]);
	dict[messageKeys.vibrationType] = parseInt(dict[messageKeys.vibrationType]);
	dict[messageKeys.appGlanceType] = parseInt(dict[messageKeys.appGlanceType]);
	dict[messageKeys.bottomTextType] = parseInt(dict[messageKeys.bottomTextType]);
	
	// Log all the settings for fun
// 	console.log('The reminderHours sent to Pebble is ' + dict[messageKeys.reminderHours] + '.');
// 	console.log('The backgroundColor sent to Pebble is ' + dict[messageKeys.backgroundColor] + '.');
// 	console.log('The circleColor sent to Pebble is ' + dict[messageKeys.circleColor] + '.');
// 	console.log('The vibrationEnabled sent to Pebble is ' + dict[messageKeys.vibrationEnabled] + '.');
// 	console.log('The vibrationType sent to Pebble is ' + dict[messageKeys.vibrationType] + '.');
// 	console.log('The displayText sent to Pebble is ' + dict[messageKeys.displayText] + '.');
// 	console.log('The rememberDuration sent to Pebble is ' + dict[messageKeys.rememberDuration] + '.');
// 	console.log('The reminderHoursStart sent to Pebble is ' + dict[messageKeys.reminderHoursStart] + '.');
// 	console.log('The breathsPerMinute sent to Pebble is ' + dict[messageKeys.breathsPerMinute] + '.');
// 	console.log('The heartRateVariation sent to Pebble is ' + dict[messageKeys.heartRateVariation] + '.');
// 	console.log('The appGlanceEnabled sent to Pebble is ' + dict[messageKeys.appGlanceEnabled] + '.');
// 	console.log('The appGlanceType sent to Pebble is ' + dict[messageKeys.appGlanceType] + '.');
// 	console.log('The achievementsEnabled sent to Pebble is ' + dict[messageKeys.achievementsEnabled] + '.');
// 	console.log('The bottomTextType sent to Pebble is ' + dict[messageKeys.bottomTextType] + '.');
	
	// Send settings values to watch side
	Pebble.sendAppMessage(dict, function(e) {
		console.log('Sent config data to Pebble');
	}, function(e) {
		console.log('Failed to send config data!');
		console.log(JSON.stringify(e));
	});
});