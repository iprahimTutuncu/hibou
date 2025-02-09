#pragma once
#include "widget.h" 

#include <functional>

namespace Thsan::UI {

	enum class Action {
		nothing,
		pressed,
		hover,
		crossed
	};

	enum class Direction {
		up	  = 0,
		down  = 1,
		left  = 2,
		right = 3
	};

	class THSAN_API Button: public Widget
	{
	public:
		virtual void connect(Action action, std::function<void()> functor) = 0;

		virtual void call(Action action) = 0;
		virtual Action lastCall() const = 0;

		virtual void set(Direction direction, const std::shared_ptr<Button>& button) = 0;
		virtual std::shared_ptr<Button> get(Direction direction) const = 0;
	};


}