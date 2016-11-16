module.exports = [
	{
		"type": "heading",
		"defaultValue": "Ajustes"
	},
	{
		"type": "text",
		"defaultValue": "Esta es la página de ajustes para el app Breathe. Estás usando la version 0.2 del app.",
	},
	{
		"type": "section",
		"items": [
			{"type": "heading",
			 "defaultValue": "Colores"
			},
			{
				"type": "color",
				"messageKey": "backgroundColor",
				"defaultValue": "000000",
				"label": "Color de fondo",
				"sunlight": true,
				"layout": "BLACK_WHITE"
			},
			{
				"type": "color",
				"messageKey": "circleColor",
				"defaultValue": "00AA55",
				"label": "Color del circulo",
				"sunlight": true,
				"capabilities": ["COLOR"]
			}
		]
	},
	{
		"type": "section",
		"items": [
			{"type": "heading",
			 "defaultValue": "Vibraciones"
			},
			{
				"type": "toggle",
				"messageKey": "vibrationEnabled",
				"defaultValue": true,
				"label": "¿Activar les vibraciones durante la respiración?",
			}
		]
	},
	{
		"type": "section",
		"capabilities": ["NOT_PLATFORM_APLITE", "NOT_PLATFORM_BASALT", "NOT_PLATFORM_CHALK"],
		"items": [
			{"type": "heading",
			 "defaultValue": "Salud"
			},
			{
				"type": "toggle",
				"messageKey": "heartRateEnabled",
				"defaultValue": true,
				"label": "¿Mostrar el ritmo cardiaco?",
				"description": "Si activado, el app mostrará el ritmo cardiaco en el menú principal. Si no, el app mostrará el número de pasos que has tomado hoy."
			}
		]
	},
	{
		"type": "section",
		"items": [
			{
				"type": "heading",
			 	"defaultValue": "Recordatorios"
			},
			{
				"type": "select",
				"messageKey": "reminderHours",
				"defaultValue": "4",
				"label": "Recuerdame cada...",
				"description": "El app te recordará a este intervalo diariamente, de 8:00 de la mañana hasta 8:00 de la tarde.",
				"options": [
					{ 
						"label": "2 horas", 
						"value": "2" 
					},
					{ 
						"label": "4 horas",
						"value": "4" 
					},
					{ 
						"label": "6 horas",
						"value": "6" 
					},
					{ 
						"label": "No me recuerda",
						"value": "0" 
					}
				]
			}
		]
	},
	{
	"type": "submit",
	"defaultValue": "Guardar mis ajustes"
	}
];