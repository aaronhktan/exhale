module.exports = [
	{
		"type": "heading",
		"defaultValue": "Configuration Page"
	},
	{
		"type": "text",
		"defaultValue": "This is the settings page for the Breathe app. You are running version 0.2.7 of Breathe.",
	},
	{
		"type": "section",
		"items": [
			{"type": "heading",
			 "defaultValue": "Colors"
			},
			{
				"type": "color",
				"messageKey": "backgroundColor",
				"defaultValue": "000000",
				"label": "Background Color",
				"sunlight": true,
				"layout": "BLACK_WHITE"
			},
			{
				"type": "color",
				"messageKey": "circleColor",
				"defaultValue": "00AA55",
				"label": "Circle Color",
				"sunlight": true,
				"capabilities": ["COLOR"]
			}
		]
	},
	{
		"type": "section",
		"items": [
			{"type": "heading",
			 "defaultValue": "In-App"
			},
			{
				"type": "toggle",
				"messageKey": "rememberDuration",
				"defaultValue": false,
				"label": "Remember last duration when launching",
			},
			{
				"type": "toggle",
				"messageKey": "vibrationEnabled",
				"defaultValue": true,
				"label": "Enable vibrations on inhale",
			}
		]
	},
	{
		"type": "section",
		"capabilities": ["NOT_PLATFORM_APLITE", "NOT_PLATFORM_BASALT", "NOT_PLATFORM_CHALK"],
		"items": [
			{"type": "heading",
			 "defaultValue": "Health"
			},
			{
				"type": "toggle",
				"messageKey": "heartRateEnabled",
				"defaultValue": true,
				"label": "Show heart rate",
				"description": "If enabled, the app shows the heart rate. If disabled, the app shows the number of steps taken today."
			}
		]
	},
	{
		"type": "section",
		"items": [
			{
				"type": "heading",
			 "defaultValue": "Reminders"
			},
			{
				"type": "select",
				"messageKey": "reminderHours",
				"defaultValue": "4",
				"label": "Remind to breathe...",
				"description": "The app will remind you to breathe at these intervals.",
				"options": [
					{
						"label": "Every hour",
						"value": "1"
					},
					{ 
						"label": "Every 2 hours", 
						"value": "2" 
					},
					{ 
						"label": "Every 3 hours",
						"value": "3" 
					},
					{ 
						"label": "Every 4 hours",
						"value": "4" 
					},
					{ 
						"label": "Every 6 hours",
						"value": "6" 
					},
					{ 
						"label": "Don't remind me",
						"value": "0" 
					}
				]
			},
			{
				"type": "slider",
				"messageKey": "reminderHoursStart",
				"defaultValue": 8,
				"label": "Time to start reminders",
				"description": "Starting at this time in the morning (i.e. 8 = 8AM) and ending 12 hours later, the app will remind you to breathe.",
				"min": 6,
				"max": 10,
				"step": 1
			}
		]
	},
	{
		"type": "text",
		"defaultValue": "<em>&nbsp;&nbsp;Many thanks to the beta testers: <br>&emsp;Ayush Gupta, and David Voicu</em>",
	},
	{
	"type": "submit",
	"defaultValue": "Save Settings"
	}
];