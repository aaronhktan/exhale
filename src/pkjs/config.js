module.exports = [
	{
		"type": "heading",
		"defaultValue": "Configuration Page"
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
			}
		]
	},
	{
	"type": "submit",
	"defaultValue": "Save Settings"
	}
];