module.exports = [
	{
		"type": "heading",
		"defaultValue": "Configuration Page"
	},
	{
		"type": "text",
		"defaultValue": "This is the settings page for the Breathe app. You are running version 2.5 of Breathe.",
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
				"defaultValue": "00AAFF",
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
				"description": "Rumble is a series of short vibrations, Pulse is double tap.",
				"options": [
					{
						"label": "Rumble - inhale only",
						"value": "0"
					},
					{
						"label": "Rumble - inhale and exhale",
						"value": "1"
					},
					{ 
						"label": "Pulse", 
						"value": "2"
					}
				]
			},
			{
				"type": "toggle",
				"capabilities": [
					"NOT_PLATFORM_APLITE", 
					"NOT_PLATFORM_BASALT",
					"NOT_PLATFORM_CHALK"
					],
				"messageKey": "heartRateVariation",
				"defaultValue": false,
				"label": "Choose breathing speed depending on heart rate?",
				"description": "Note: This requires a watch with a heart rate monitor."
			},
			{
				"type": "slider",
				"messageKey": "breathsPerMinute",
				"defaultValue": 7,
				"label": "Breaths per minute",
				"min": 2,
				"max": 10,
				"step": 1
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
		"type": "section",
		"capabilities": ["NOT_PLATFORM_APLITE"],
		"items": [
			{
				"type": "heading",
				"defaultValue": "App Glance",
			},
			{
				"type": "toggle",
				"messageKey": "appGlanceEnabled",
				"defaultValue": true,
				"label": "Display app glance text in the app menu",
			},
			{
				"type": "select",
				"messageKey": "appGlanceType",
				"defaultValue": "0",
				"label": "App Glance Type",
				"options": [
					{
						"label": "Last session time",
						"value": "0"
					},
					{
						"label": "Current daily total",
						"value": "1"
					},
					{
						"label": "Streak",
						"capabilities": ["NOT_PLATFORM_APLITE"],
						"value": "2"
					}
				]
			},
		]
	},
	{
		"type": "section",
		"capabilities": ["HEALTH"],
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
						"label": "Nothing",
						"value": "0"
					},
					{
						"label": "Greeting",
						"value": "1"
					},
					{ 
						"label": "Steps today", 
						"value": "2" 
					},
					{ 
						"label": "Heart Rate",
						"capabilities": ["PLATFORM_DIORITE", "PLATFORM_EMERY"],
						"value": "3" 
					}
				]
			},
		]
	},
	{
		"type": "section",
		"capabilities": ["NOT_PLATFORM_APLITE"],
		"items": [
			{
				"type": "heading",
				"defaultValue": "Achievements",
			},
			{
				"type": "toggle",
				"messageKey": "achievementsEnabled",
				"defaultValue": true,
			"label": "Enable achievements"
			},
			{
				"type": "select",
				"messageKey": "bottomTextType",
				"defaultValue": "0",
				"label": "Bottom Text Type",
				"options": [
					{
						"label": "Total today",
						"value": "0"
					},
					{
						"label": "Streak",
						"value": "1"
					}
				]
			},
			{
				"type": "input",
				"messageKey": "achievementsBackup",
				"defaultValue": "0000000000000000000000000000000000000",
				"label": "Achievement Backup / Restore",
				"description": "This text allows you to back up the achievements that you have earned, your current and record streak, as well as the total number of minutes you have breathed with this app. Keep it somewhere safe! If you want to restore with a previously saved text, paste it into the textbox and tap the \"Save Settings\" button at the bottom of the page."
			},
			{
				"type": "button",
				"primary": false,
				"defaultValue": "Reset achievements",
				"id": "resetAchievementsButton",
			}
		]
	},
	{
		"type": "text",
		"defaultValue": "<em><center>Many thanks to the beta testers and contributors: <br>Paula&nbsp;Bosca, Nikita&nbsp;Cheng, Ayush&nbsp;Gupta, Ellen&nbsp;Huang, Yvonne&nbsp;Tan, David&nbsp;Voicu, /u/dryingsocks, /u/JohnEdwa, and /u/PiwwowPants.</center></em>",
	},
	{
		"type": "submit",
		"defaultValue": "Save Settings"
	},
];