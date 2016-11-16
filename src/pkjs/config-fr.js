module.exports = [
	{
		"type": "heading",
		"defaultValue": "Paramètres"
	},
	{
		"type": "text",
		"defaultValue": "Ceci est la page de configuration pour l'app Breathe. Vous utilisez version 0.2 de l'app.",
	},
	{
		"type": "section",
		"items": [
			{"type": "heading",
			 "defaultValue": "Couleurs"
			},
			{
				"type": "color",
				"messageKey": "backgroundColor",
				"defaultValue": "000000",
				"label": "Couleur de fond",
				"sunlight": true,
				"layout": "BLACK_WHITE"
			},
			{
				"type": "color",
				"messageKey": "circleColor",
				"defaultValue": "00AA55",
				"label": "Couleur de cercle",
				"sunlight": true,
				"capabilities": ["COLOR"]
			}
		]
	},
	{
		"type": "section",
		"items": [
			{"type": "heading",
			 "defaultValue": "Vibrations"
			},
			{
				"type": "toggle",
				"messageKey": "vibrationEnabled",
				"defaultValue": true,
				"label": "Activer les vibrations pendant respirations?",
			}
		]
	},
	{
		"type": "section",
		"capabilities": ["NOT_PLATFORM_APLITE", "NOT_PLATFORM_BASALT", "NOT_PLATFORM_CHALK"],
		"items": [
			{"type": "heading",
			 "defaultValue": "Santé"
			},
			{
				"type": "toggle",
				"messageKey": "heartRateEnabled",
				"defaultValue": true,
				"label": "Montrer le rhythme cardiaque",
				"description": "Si activé, l'app montre le rhythme cardiaque dans le menu principal. Sinon, le nombre de pas marchés aujourd'hui sera montré."
			}
		]
	},
	{
		"type": "section",
		"items": [
			{
				"type": "heading",
			 	"defaultValue": "Rappels"
			},
			{
				"type": "select",
				"messageKey": "reminderHours",
				"defaultValue": "4",
				"label": "Me rappeler toutes les...",
				"description": "L'app vous rappellera de respirer à cet intervalle tous les jours, à partir de 8h00 jusqu'à 20h00.",
				"options": [
					{ 
						"label": "2 heures", 
						"value": "2" 
					},
					{ 
						"label": "4 heures",
						"value": "4" 
					},
					{ 
						"label": "6 heures",
						"value": "6" 
					},
					{ 
						"label": "Ne me rappelle pas",
						"value": "0" 
					}
				]
			}
		]
	},
	{
	"type": "submit",
	"defaultValue": "Sauvegarder mes paramètres"
	}
];