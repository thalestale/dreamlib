#if 0
//#ifndef DREAMLIB_ITEM_H
//#define DREAMLIB_ITEM_H

#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/node2d.hpp>

namespace godot {
	class DreamItem : public Label {
		GDCLASS(DreamItem, Label)

	private:
		String item_name;
		int quantity;
		StringName item_type;

		Node2D* cursor_node;
		Label* tip_label;

	protected:
		static void _bind_methods();
	public:
		DreamItem();
		~DreamItem();

		void _notification(int p_what);

		String get_item_name() const;
		int get_quantity() const;
		StringName get_item_type() const;

		void set_item_name(String p_value);
		void set_quantity(int p_value);
		void set_item_type(StringName p_value);

		void item_focused();
		void item_unfocused();
		void item_pressed();

		void set_cursor_node(Node2D *p_value);
		Node2D *get_cursor_node();

		void set_tip_label(Label *p_value);
		Label *get_tip_label();

	};
}

#endif // !DREAMLIB_ITEM_H
