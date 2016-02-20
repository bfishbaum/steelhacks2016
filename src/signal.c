#include <pebble.h>

/*  this code sends a message to the corresponding android
	app whenever the pebble determines that there may be
	a medical emergency that requires immediate attention
*/

static string APIKey = "";
int main(void){
	app_message_open();
		

}

void send_data_to_app(bool fell){
	Pebble.sendAppMessage({"Fell": fell});
}


