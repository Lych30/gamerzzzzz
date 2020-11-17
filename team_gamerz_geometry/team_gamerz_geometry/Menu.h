#pragma once
class Button {
public:
	sf::RectangleShape ButtonShape;
	sf::Text text;
	Button() {
		this->ButtonShape.setFillColor(sf::Color(0, 27, 55));
		this->ButtonShape.setSize(sf::Vector2f(400.f, 150.f));
	}
};
std::vector<Button>InitialiseButton(sf::RenderWindow* window, int xOffset, int yOffset, std::vector<Button> buttons, std::string text) {
	Button button;
	button.ButtonShape.setPosition((window->getSize().x / 2) + xOffset, (window->getSize().y / 3) + yOffset);
	button.text.setString(text);
	button.text.setScale(2, 2);
	//button.text.setOrigin(button.ButtonShape.getPosition());
	button.text.setPosition(button.ButtonShape.getPosition().x, button.ButtonShape.getPosition().y);
	buttons.push_back(Button(button));
	std::cout << buttons.size() << std::endl;
	return buttons;
}
