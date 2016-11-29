module.exports = [
	{
		"type": "heading",
		"defaultValue": "Configuration Page"
	},
	{
		"type": "text",
		"defaultValue": "This is the settings page for the Breathe app. You are running version 1.0 of Breathe.",
	},
	{
		"type": "section",
		"items": [
			{
				"type": "heading",
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
			{
				"type": "heading",
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
			},
			{
				"type": "select",
				"messageKey": "vibrationType",
				"defaultValue": "0",
				"label": "Vibration Type",
				"description": "Shakes is like the Apple Watch, Taps is double tap.",
				"options": [
					{
						"label": "Shakes",
						"value": "0"
					},
					{ 
						"label": "Taps", 
						"value": "1"
					}
				]
			},
			{
				"type": "slider",
				"messageKey": "breathsPerMinute",
				"defaultValue": 7,
				"label": "Breaths per minute",
				"min": 4,
				"max": 10,
				"step": 1
			}
		]
	},
	{
		"type": "section",
		"capabilities": ["NOT_PLATFORM_APLITE"],
		"items": [
			{
				"type": "heading",
			 	"defaultValue": "Health"
			},
			{
				"type": "select",
				"messageKey": "displayText",
				"defaultValue": "1",
				"label": "Show...",
				"description": "This determines what the app shows at the top part of the main menu. Heart rate requires a watch with a heart rate monitor.",
				"options": [
					{
						"label": "Greeting",
						"value": "0"
					},
					{ 
						"label": "Steps today", 
						"value": "1" 
					},
					{ 
						"label": "Heart Rate",
						"value": "2" 
					}
				]
			},
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
		"defaultValue": "<em><center>Many thanks to the beta testers: <br>Paula&nbsp;Bosca, Nikita&nbsp;Cheng, Ayush&nbsp;Gupta, Ellen&nbsp;Huang, Yvonne&nbsp;Tan, David&nbsp;Voicu, /u/dryingsocks, and /u/PiwwowPants.</center></em>",
	},
	{
		"type": "submit",
		"defaultValue": "Save Settings"
	}
];