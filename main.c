#include <pebble.h>
static void init(){
	//create the main window
	s_main_window = window.create();
}

static void deinit(){

}

static void main_window_load(Window *window) {

}

static void main_window_unload(Window *window) {

}

int main(void){
	init();
	app_event_loop();
	deinit();
}
