#if 0

#include "dreamlib_item.h"
#include "gde_util.h"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void DreamItem::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_quantity"), &DreamItem::get_quantity);
	ClassDB::bind_method(D_METHOD("get_item_type"), &DreamItem::get_item_type);
	ClassDB::bind_method(D_METHOD("get_item_name"), &DreamItem::get_item_name);
	ClassDB::bind_method(D_METHOD("set_quantity", "value"), &DreamItem::set_quantity);
	ClassDB::bind_method(D_METHOD("set_item_type", "value"), &DreamItem::set_item_type);
	ClassDB::bind_method(D_METHOD("set_item_name", "value"), &DreamItem::set_item_name);
	ClassDB::bind_method(D_METHOD("set_" "cursor_node", "p_value"), &DreamItem::set_cursor_node); 
	ClassDB::bind_method(D_METHOD("get_" "cursor_node"), &DreamItem::get_cursor_node);
	ClassDB::bind_method(D_METHOD("set_" "tip_label", "p_value"), &DreamItem::set_tip_label); 
	ClassDB::bind_method(D_METHOD("get_" "tip_label"), &DreamItem::get_tip_label); 

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "tip_label", PROPERTY_HINT_NODE_TYPE, "Label"), "set_" "tip_label", "get_" "tip_label");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "cursor_node", PROPERTY_HINT_NODE_TYPE, "Node2D"), "set_" "cursor_node", "get_" "cursor_node");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "quantity"), "set_quantity", "get_quantity");
	ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "item_type"), "set_item_type", "get_item_type");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "item_name"), "set_item_name", "get_item_name");

	ADD_SIGNAL(MethodInfo("item_focused", PropertyInfo(Variant::OBJECT, "node")));
	ADD_SIGNAL(MethodInfo("item_unfocused", PropertyInfo(Variant::OBJECT, "node")));
	ADD_SIGNAL(MethodInfo("item_pressed", PropertyInfo(Variant::OBJECT, "node")));
}

DreamItem::DreamItem() {
	item_name = "...";
	quantity = 1;
	item_type = "";
	cursor_node = nullptr;
	tip_label = nullptr;

}

DreamItem::~DreamItem() {}

void godot::DreamItem::_notification(int p_what){
	switch (p_what){
	case NOTIFICATION_READY:
		if (tip_label == nullptr){
			if (this->get_parent()->get_parent()->get_node_or_null("ItemTipRichTextLabel") != nullptr){
				tip_label = static_cast<Label *>(this->get_parent()->get_parent()->get_node_or_null("ItemTipRichTextLabel"));
			}
		}
		break;
	
	case NOTIFICATION_ENTER_TREE:
		this->set_focus_mode(FOCUS_ALL);
		this->set_process_mode(PROCESS_MODE_ALWAYS);
		this->connect("focus_entered", godot::Callable(this, "item_focused"));
		this->connect("focus_exited", godot::Callable(this, "item_unfocused"));
		this->connect("gui_input", godot::Callable(this, "item_pressed"));
		set_pivot_offset(get_pivot_offset() / 2.0);
		break;
	
	default:
		break;
	}
}


void DreamItem::item_focused() {
	emit_signal("item_focused", this);
}

void DreamItem::item_unfocused() {
	emit_signal("item_unfocused", this);
}

void DreamItem::item_pressed() {
	emit_signal("item_pressed", this);
}

void godot::DreamItem::set_cursor_node(Node2D *p_value){
	cursor_node = p_value;
}

Node2D *godot::DreamItem::get_cursor_node(){
    return cursor_node;
}

void godot::DreamItem::set_tip_label(Label *p_value){
	tip_label = p_value;
}

Label *godot::DreamItem::get_tip_label(){
    return tip_label;
}

void DreamItem::set_quantity(int p_value) {
	quantity = p_value;
}

void DreamItem::set_item_type(StringName p_value) {
	item_type = p_value;
}

void DreamItem::set_item_name(String p_value) {
	this->set_name(p_value);
	item_name = p_value;
}

int DreamItem::get_quantity() const {
	return quantity;
}

StringName DreamItem::get_item_type() const {
	return item_type;
}

String DreamItem::get_item_name() const {
	return item_name;
}

#endif