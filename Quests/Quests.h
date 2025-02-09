#pragma once
#include "../systems/Controller.h"
#include "../Components/Components.h"

void createDialog(EntityManager* em, RenderSystem* render);
void createDialogLayout(EntityManager* em);
void createDialogText(EntityManager* em);
void createDialogButtons(EntityManager* em);
void createOkButtonDialog(EntityManager* em);
void createCancelButtonDialog(EntityManager* em);

void initAreaForFinalQuest(Controller* controller) {
	auto [input, em, render, state, battleContext, collisionMaps, currentLocation] = controller->getAll();

	auto area = em->createEntity();
	area->addComponent<PositionComponent>(TRAINER_POSITION_X, TRAINER_POSITION_Y + TRAINER_HEIGHT);
	area->addComponent<SizeComponent>(16, 11);
	area->addComponent<GameTypeEntityComponent>();
	area->addComponent<QuestAreaComponent>();
	area->addComponent<RenderLayerComponent>(1);
    area->addComponent<ShapeComponent>(ShapeType::Ellipse, sf::Color(68, 68, 68, 204));
}

void finalQuestCollision(Controller* controller) {
    auto [input, em, render, state, battleContext, maps, currentLocation] = controller->getAll();
    auto keys = input->getPressedKeys();
    
    if (std::find(keys.begin(), keys.end(), sf::Keyboard::Enter) == keys.end()) return;
    
    auto player = em->getEntitiesWithComponent<PlayerControlComponent>()[0];
    auto questAreas = em->getEntitiesWithComponent<QuestAreaComponent>();

    if (questAreas.empty()) return;
    auto questArea = questAreas[0];

    if (isCollision(player, questArea)) {
        *state = GameState::Dialog;
        createDialog(em, render);
        render->addEntities(em->getEntitiesWithComponent<QuestDialogComponent>());
        input->clear();
    }
}

void createDialog(EntityManager* em, RenderSystem* render) {
	createDialogLayout(em);
	createDialogText(em);
	createDialogButtons(em);
}

void createDialogLayout(EntityManager* em) {
	auto dialog = em->createEntity();
	dialog->addComponent<PositionComponent>(DIALOG_POSITION_X, DIALOG_POSITION_Y);
	dialog->addComponent<SizeComponent>(DIALOG_WIDTH, DIALOG_HEIGHT);
	dialog->addComponent<QuestDialogComponent>();
	dialog->addComponent<RenderLayerComponent>(1);
	sf::Texture dialogTexture;

	if (dialogTexture.loadFromFile("../res/background_menu(62x46).png")) {
		dialog->addComponent<TextureComponent>(
			dialogTexture, 
			62, 
			46
		);
	}
}

void createDialogText(EntityManager* em) {
	auto dialog = em->createEntity();
	dialog->addComponent<PositionComponent>(DIALOG_POSITION_X + 30, DIALOG_POSITION_Y + 30);
	dialog->addComponent<SizeComponent>(DIALOG_WIDTH - 30, DIALOG_HEIGHT - 30);
	dialog->addComponent<QuestDialogComponent>();
	dialog->addComponent<RenderLayerComponent>(2);
	dialog->addComponent<TextComponent>(
		"Hello, how are you?", 
		DIALOG_POSITION_X + 30,
		DIALOG_POSITION_Y + 30,
		20,
		sf::Color(68, 68, 68)
	);
}


void createDialogButtons(EntityManager* em) {
	createOkButtonDialog(em);
	createCancelButtonDialog(em);
}

void createOkButtonDialog(EntityManager* em) {
	auto okButton = em->createEntity();
	okButton->addComponent<PositionComponent>(DIALOG_OK_BUTTON_POSITION_X, DIALOG_OK_BUTTON_POSITION_Y);
	okButton->addComponent<SizeComponent>(DIALOG_BUTTON_WIDTH, DIALOG_BUTTON_HEIGHT);
	okButton->addComponent<QuestDialogComponent>();
	okButton->addComponent<RenderLayerComponent>(2);
	sf::Texture okButtonTexture;
	if (okButtonTexture.loadFromFile("../res/okButton(32x13).png")) {
		okButton->addComponent<TextureComponent>(
			okButtonTexture, 
			32, 
			13
		);
	}
}

void createCancelButtonDialog(EntityManager* em) {
	auto cancelButton = em->createEntity();
	cancelButton->addComponent<PositionComponent>(DIALOG_CANCEL_BUTTON_POSITION_X, DIALOG_CANCEL_BUTTON_POSITION_Y);
	cancelButton->addComponent<SizeComponent>(DIALOG_BUTTON_WIDTH, DIALOG_BUTTON_HEIGHT);
	cancelButton->addComponent<QuestDialogComponent>();
	cancelButton->addComponent<RenderLayerComponent>(2);
	sf::Texture cancelButtonTexture;
	if (cancelButtonTexture.loadFromFile("../res/backButton(32x13).png")) {
		cancelButton->addComponent<TextureComponent>(
			cancelButtonTexture, 
			32, 
			13
		);
	}
}
