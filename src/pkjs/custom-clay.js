module.exports = function(minified) {
	var clayConfig = this;
	var _ = minified._;
	var $ = minified.$;
	var HTML = minified.HTML;

	function toggleReminder() { // This means that reminders has been toggled
		if (this.get() !== "0") { // If reminders have been toggled to on, enable setting start time
			clayConfig.getItemByMessageKey('reminderHoursStart').enable();
		} else { // Otherwise, disable that element of the page
			clayConfig.getItemByMessageKey('reminderHoursStart').disable();
		}
	}

	function toggleVibration() { // Same thing for vibration
		if (this.get() !== false) {
			clayConfig.getItemByMessageKey('vibrationType').enable();
		} else {
			clayConfig.getItemByMessageKey('vibrationType').disable();
		}
	}

	function toggleAppGlance() {
		if (this.get() !== false) {
			clayConfig.getItemByMessageKey('appGlanceType').enable();
		} else {
			clayConfig.getItemByMessageKey('appGlanceType').disable();
		}
	}
	
	function resetAchievements() {
		clayConfig.getItemByMessageKey('achievementsBackup').set('0000000000000000000000000000000000000');
	}

	clayConfig.on(clayConfig.EVENTS.AFTER_BUILD, function() {
		var reminderSelect = clayConfig.getItemByMessageKey('reminderHours');
		toggleReminder.call(reminderSelect);
		reminderSelect.on('change', toggleReminder);
		
		var vibrationEnabledToggle = clayConfig.getItemByMessageKey('vibrationEnabled');
		toggleVibration.call(vibrationEnabledToggle);
		vibrationEnabledToggle.on('change', toggleVibration);

		var appGlanceEnabledToggle = clayConfig.getItemByMessageKey('appGlanceEnabled');
		toggleAppGlance.call(appGlanceEnabledToggle);
		appGlanceEnabledToggle.on('change', toggleAppGlance);
		
		var resetAchievementsButton = clayConfig.getItemById('resetAchievementsButton');
		resetAchievementsButton.on('click', resetAchievements);
	});
};