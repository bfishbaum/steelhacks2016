#include <pebble.h>

/*  this code sends a message to the corresponding android
	app whenever the pebble determines that there may be
	a medical emergency that requires immediate attention
*/

static void callICE(){
	DictionaryIterator* iterator;
	app_message_outbox_begin(&iterator);
	string key = "Call";
	int shouldCall = 1;
	dict_write_int(iterator, key, &shouldCall, sizeof(shouldCall), true);
	app_message_outbox_send();
}
