#pragma once
#include "../systems/Controller.h"

void initOnboarding(EntityManager* em);
void initOnboardingLayout(EntityManager* em);
void initOnboardingText(EntityManager* em);
void initOnboardingButton(EntityManager* em);

void initOnboarding(EntityManager* em) {
	initOnboardingLayout(em);
	initOnboardingText(em);
	initOnboardingButton(em);
}

void initOnboardingLayout(EntityManager* em) {
	auto onboarding = em->createEntity();
	onboarding->addComponent<PositionComponent>(
		ONBOARDING_LAYOUT_POSITION_X,
		ONBOARDING_LAYOUT_POSITION_Y
	);
	onboarding->addComponent<SizeComponent>(
		ONBOARDING_LAYOUT_WIDTH,
		ONBOARDING_LAYOUT_HEIGHT
	);
	onboarding->addComponent<RenderLayerComponent>(2);
	onboarding->addComponent<GameTypeEntityComponent>();
	sf::Texture onboardingTexture;
	if (onboardingTexture.loadFromFile("../res/background_menu(62x46).png")) {
		onboarding->addComponent<TextureComponent>(
			onboardingTexture,
			62, 
			46
		);
	}
}

void initOnboardingText(EntityManager* em) {
	auto text = em->createEntity();
	text->addComponent<PositionComponent>(
		ONBOARDING_TEXT_X,
		ONBOARDING_TEXT_Y
	);
	text->addComponent<RenderLayerComponent>(3);
	text->addComponent<GameTypeEntityComponent>();
	text->addComponent<TextComponent>(
		ONBOARDING_TEXT_VALUE,
		ONBOARDING_TEXT_X,
		ONBOARDING_TEXT_Y,
		ONBOARDING_TEXT_SIZE,
		sf::Color(68, 68, 68)
	);
}

void initOnboardingButton(EntityManager* em) {
	auto button = em->createEntity();
	button->addComponent<PositionComponent>(
		ONBOARDING_BUTTON_X,
		ONBOARDING_BUTTON_Y
	);
	button->addComponent<SizeComponent>(
		ONBOARDING_BUTTON_WIDTH,
		ONBOARDING_BUTTON_HEIGHT
	);
	button->addComponent<RenderLayerComponent>(3);
	button->addComponent<GameTypeEntityComponent>();
	sf::Texture buttonTexture;
	if (buttonTexture.loadFromFile("../res/okButton(32x13).png")) {
		button->addComponent<TextureComponent>(
			buttonTexture,
			32,
			13
		);
	}
}