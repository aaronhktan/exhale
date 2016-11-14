module.exports = [
	{
		"type": "heading",
		"defaultValue": "Configuration Page"
	},
	{
		"type": "text",
		"defaultValue": "This is the settings page. Feel free to change anything you want. You are running version 0.1.2 of Breathe.",
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
			 "defaultValue": "Haptic Feedback"
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
	"type": "submit",
	"defaultValue": "Save Settings"
	}
];