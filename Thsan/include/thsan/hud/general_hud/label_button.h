#pragma once
#include "label.h"
#include "Button.h"
#include <thsan/graphics/drawable/text.h>

namespace Thsan
{
	class Text;
	class Font;
}

namespace Thsan::UI {

	class THSAN_API LabelButton : public Button
	{
	public:
		virtual void setString(const std::u32string& text) = 0;
		virtual const std::u32string getString() const = 0;
		virtual void setFont(std::shared_ptr<Thsan::Font> font) = 0;
		virtual void enableCustomFont() = 0;
		virtual void disableCustomFont() = 0;
		virtual void setCustomFont(std::shared_ptr<Thsan::Font> font) = 0;
		virtual void setColor(tsm::Color color) = 0;
		virtual void setSizeToFontSize() = 0;
		virtual glm::ivec2 getFontSize() = 0;
		virtual tsm::Color getColor() const = 0;

		static std::shared_ptr<LabelButton> create(std::string name);
	};


	class LabelButtonImpl : public LabelButton
	{
	public:

		LabelButtonImpl(std::string name);

		// Implement abstract methods from Button
		void connect(Action action, std::function<void()> functor) override;
		void call(Action action) override;
		Action lastCall() const override;
		void set(Direction direction, const std::shared_ptr<Button>& button) override;
		std::shared_ptr<Button> get(Direction direction) const override;

		//inherited from Label
		virtual void setString(const std::u32string& text) override;
		virtual const std::u32string getString() const override;
		void enableCustomFont() override;
		void disableCustomFont() override;

		void setSizeToFontSize() override;
		glm::ivec2 getFontSize() override;

		virtual void setFont(std::shared_ptr<Thsan::Font> font) override;
		void setCustomFont(std::shared_ptr<Thsan::Font> font) override;

		virtual void setColor(tsm::Color color) override;
		virtual tsm::Color getColor() const override;

		// Implement abstract methods from Widget
		virtual void update(float deltaTime) override;

		void remove(const std::shared_ptr<Widget>& widget) override;

		void setMargin(glm::vec2 margin) override;
		void setSize(glm::vec2 size) override;
		void setAnchor(Anchor anchor) override;

		glm::vec2 getMargin() const override;
		glm::vec2 getGlobalPosition() const override;
		glm::vec2 getSize() const override;

		Anchor getAnchor() const override;

		void hide() override;
		void show() override;
		bool isHidden() const override;

		WidgetData& getWidgetData() override;

		std::string getName() override;
		std::shared_ptr<Mesh> getMesh() const override;


	private:
		void draw(const RenderTarget& target, RenderStates& states) const override;

		bool hasCustomFont{ false };
		std::shared_ptr<Thsan::Font> customFont;
		std::shared_ptr<Thsan::Font> defaultFont;

		std::unordered_map<Action, std::function<void()>> actions;
		std::unordered_map<Direction, std::weak_ptr<Button>> directionalButtons;
		Action lastAction{ Action::nothing };

		WidgetData widgetData;

		std::vector<std::shared_ptr<Widget>> childs;
		std::weak_ptr<Widget> parent;

		float elapsedTime{ 0.0f };
		std::string name;

		std::shared_ptr<Thsan::Text> text;
	};

}

