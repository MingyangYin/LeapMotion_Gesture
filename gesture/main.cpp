#include"SampleListener.h"




int main(int argc, char* argv[]) {
	// Create a sample listener and controller
	//Êý¾Ý´æ´¢
	
	SampleListener listener;
	Controller controller;

	// Have the sample listener receive events from the controller
	controller.addListener(listener);

	if (argc > 1 && strcmp(argv[1], "--bg") == 0)
		controller.setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);
	controller.setPolicy(Leap::Controller::POLICY_IMAGES);
	controller.setPolicy(Leap::Controller::POLICY_ALLOW_PAUSE_RESUME);

	// Keep this process running until Enter is pressed
	std::cout << "Press Enter to quit, or enter 'p' to pause or unpause the service..." << std::endl;

	bool paused = false;
	while (true) {
		char c = std::cin.get();
		if (c == 'p') {
			paused = !paused;
			controller.setPaused(paused);
			std::cin.get(); //skip the newline
		}
		else
			break;
	}

	// Remove the sample listener when done
	controller.removeListener(listener);

	return 0;
}

