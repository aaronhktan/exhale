module.exports = [
	{
		"type": "heading",
		"defaultValue": "Paramètres"
	},
	{
		"type": "text",
		"defaultValue": "Ceci est la page de configuration pour l'app Breathe. Vous utilisez version 0.2.7 de l'app.",
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
				"label": "Couleur du cercle",
				"sunlight": true,
				"capabilities": ["COLOR"]
			}
		]
	},
	{
		"type": "section",
		"items": [
			{"type": "heading",
			 "defaultValue": "Dans l'app"
			},
			{
				"type": "toggle",
				"messageKey": "rememberDuration",
				"defaultValue": false,
				"label": "Se rappeler de la durée précédente?",
			},
			{
				"type": "toggle",
				"messageKey": "vibrationEnabled",
				"defaultValue": true,
				"label": "Activer les vibrations pendant les respirations?",
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
				"label": "Me rappeler de respirer",
				"description": "L'app vous rappellera de respirer à cet intervalle tous les jours.",
				"options": [
					{ 
						"label": "Toutes les heures", 
						"value": "1" 
					},
					{ 
						"label": "Toutes les 2 heures", 
						"value": "2" 
					},
					{ 
						"label": "Toutes les 3 heures",
						"value": "3" 
					},
					{ 
						"label": "Toutes les 4 heures",
						"value": "4" 
					},
					{ 
						"label": "Toutes les 6 heures",
						"value": "6" 
					},
					{ 
						"label": "Ne me rappelle pas",
						"value": "0" 
					}
				]
			},
			{
				"type": "slider",
				"messageKey": "reminderHoursStart",
				"defaultValue": 8,
				"label": "Quand devraient commencer les rappels?",
				"description": "Dès cette heure du matin (c.à.d 8 = 8h00 du matin) et pour un période 12 heures de suite, l'app vous rappellera de respirer.",
				"min": 6,
				"max": 10,
				"step": 1
			}
		]
	},
	{
		"type": "text",
		"defaultValue": "<em>&nbsp;&nbsp;Un grand merci aux bêta-testeurs: <br>&nbsp;Ayush Gupta, et David Voicu</em>",
	},
	{
	"type": "submit",
	"defaultValue": "Sauvegarder mes paramètres"
	}
];