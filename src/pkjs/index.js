// Import the Clay package
var Clay = require('pebble-clay');
// Import messageKeys module to log data
var messageKeys = require('message_keys');
// Load our Clay configuration file
var clayConfig = require('./config.js');
var clayConfigFR = require('./config-fr.js');
var clayConfigES = require('./config-es.js');
// Initialize Clay
var clay = new Clay(clayConfig, null, { autoHandleEvents: false });

Pebble.addEventListener('showConfiguration', function(e) {
	var info = Pebble.getActiveWatchInfo();
	console.log('The language of the phone is ' + info.language.substr(0, 2));
	if (info.language.substr(0, 2) == 'fr') {
		clay.config = clayConfigFR; 
	} else if (info.language.substr(0, 2) == 'es') {
		clay.config = clayConfigES;
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
	var platform = clay.meta.activeWatchInfo.platform;
	if (platform === 'aplite' || platform === 'basalt' || platform === 'chalk') {
		dict[messageKeys.heartRateEnabled] = false;
	}
	console.log('The reminderHours sent to Pebble is ' + dict[messageKeys.reminderHours] + '.');
	console.log('The backgroundColor sent to Pebble is ' + dict[messageKeys.backgroundColor] + '.');
	console.log('The circleColor sent to Pebble is ' + dict[messageKeys.circleColor] + '.');
	console.log('The vibrationEnabled sent to Pebble is ' + dict[messageKeys.vibrationEnabled] + '.');
	console.log('The heartRateEnabled sent to Pebble is ' + dict[messageKeys.heartRateEnabled] + '.');
	
  // Send settings values to watch side
  Pebble.sendAppMessage(dict, function(e) {
    console.log('Sent config data to Pebble');
  }, function(e) {
    console.log('Failed to send config data!');
    console.log(JSON.stringify(e));
  });
});