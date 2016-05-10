#include "GraphicalRun.h"
#include "GUI.h"

float GraphicalRun::Simulate() {

	sf::Clock SimulationClock;
		
	bool CollisionDetection = false;

	float ClosestDistance = sqrt(pow(Params::WindowHeight, 2) + pow(Params::WindowWidth, 2));


	sf::Clock Clock;

	float PhysicsTimeStepAccumulator = 0.0f;

	while (Window.isOpen() && !CollisionDetection && !OutOfBounds()) {

		const sf::Time FrameTime = Clock.restart();
		float FrameSeconds = FrameTime.asSeconds();
		fps = 1 / FrameSeconds;	// pl. 60 -szor lesz meg benne

		if (FrameSeconds > 0.1f) {
			FrameSeconds = 0.1f;
		}

		PhysicsTimeStepAccumulator += FrameSeconds;

		HandleUserInput();

		while (PhysicsTimeStepAccumulator >= Params::PhysicsTimeStep) {	// PhysicsTimeStep = 1/60	// szive lelke

			Interceptor.SetNNControls(&TheNet);

			Interceptor.update();
			Bandit.update();


			CollisionDetection = Interceptor.collision(&Bandit);	//RocketController.cpp

			PhysicsTimeStepAccumulator -= Params::PhysicsTimeStep;
		}


		Window.clear(sf::Color::Black);

		//draw here//
		Interceptor.draw(Window);
		Bandit.draw(Window);
		GUI (Window, iGeneration, iGenome);

		Window.display();
		
	}

	float SimulationTime = SimulationClock.restart().asSeconds();
	
	return (CollisionDetection * (1 - SimulationTime / 10) + !CollisionDetection * (1 / ClosestDistance * SimulationTime));
	
	//return (CollisionDetection * SimulationTime + !CollisionDetection * ClosestDistance) / SimulationTime;

	//return (CollisionDetection * (100 - SimulationTime) + !CollisionDetection * (1 / ClosestDistance * SimulationTime));
}

void GraphicalRun::HandleUserInput() {

	sf::Event event;

	while (Window.pollEvent(event)) {
		
		switch (event.type) {
		
		case sf::Event::Closed:
			Window.close();
			break;
		case sf::Event::KeyPressed:
			
			pressedKeys[event.key.code] = true;
			
			switch (event.key.code) {
			case sf::Keyboard::S:
				SaveAll();
				break;
			case sf::Keyboard::L:
				Interceptor.reset();
				Bandit.reset();
				LoadAll();
				break;
			case sf::Keyboard::Escape:
				Window.close();
				break;
			}
			break;
		}
	}
}