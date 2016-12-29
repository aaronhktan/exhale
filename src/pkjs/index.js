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
var clay = new Clay(clayConfig, customClay, { autoHandleEvents: false });

Pebble.addEventListener('showConfiguration', function(e) {
	var info = Pebble.getActiveWatchInfo();
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
	console.log('The reminderHours sent to Pebble is ' + dict[messageKeys.reminderHours] + '.');
	console.log('The backgroundColor sent to Pebble is ' + dict[messageKeys.backgroundColor] + '.');
	console.log('The circleColor sent to Pebble is ' + dict[messageKeys.circleColor] + '.');
	console.log('The vibrationEnabled sent to Pebble is ' + dict[messageKeys.vibrationEnabled] + '.');
	console.log('The vibrationType sent to Pebble is ' + dict[messageKeys.vibrationType] + '.');
	console.log('The displayText sent to Pebble is ' + dict[messageKeys.displayText] + '.');
	console.log('The rememberDuration sent to Pebble is ' + dict[messageKeys.rememberDuration] + '.');
	console.log('The reminderHoursStart sent to Pebble is ' + dict[messageKeys.reminderHoursStart] + '.');
	console.log('The breathsPerMinute sent to Pebble is ' + dict[messageKeys.breathsPerMinute] + '.');
	console.log('The heartRateVariation sent to Pebble is ' + dict[messageKeys.heartRateVariation] + '.');
	console.log('The appGlanceEnabled sent to Pebble is ' + dict[messageKeys.appGlanceEnabled] + '.');
	console.log('The appGlanceType sent to Pebble is ' + dict[messageKeys.appGlanceType] + '.');
	console.log('The achievementsEnabled sent to Pebble is ' + dict[messageKeys.achievementsEnabled] + '.');
	console.log('The bottomTextType sent to Pebble is ' + dict[messageKeys.bottomTextType] + '.');
	
	// Send settings values to watch side
	Pebble.sendAppMessage(dict, function(e) {
		console.log('Sent config data to Pebble');
	}, function(e) {
		console.log('Failed to send config data!');
		console.log(JSON.stringify(e));
	});
});