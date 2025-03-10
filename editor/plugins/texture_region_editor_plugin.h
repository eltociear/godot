/*************************************************************************/
/*  texture_region_editor_plugin.h                                       */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#ifndef TEXTURE_REGION_EDITOR_PLUGIN_H
#define TEXTURE_REGION_EDITOR_PLUGIN_H

#include "canvas_item_editor_plugin.h"
#include "editor/editor_plugin.h"
#include "scene/2d/sprite_2d.h"
#include "scene/3d/sprite_3d.h"
#include "scene/gui/nine_patch_rect.h"
#include "scene/resources/style_box.h"
#include "scene/resources/texture.h"

class ViewPanner;

class TextureRegionEditor : public VBoxContainer {
	GDCLASS(TextureRegionEditor, VBoxContainer);

	enum SnapMode {
		SNAP_NONE,
		SNAP_PIXEL,
		SNAP_GRID,
		SNAP_AUTOSLICE
	};

	friend class TextureRegionEditorPlugin;
	OptionButton *snap_mode_button = nullptr;
	Button *zoom_in = nullptr;
	Button *zoom_reset = nullptr;
	Button *zoom_out = nullptr;
	HBoxContainer *hb_grid = nullptr; //For showing/hiding the grid controls when changing the SnapMode
	SpinBox *sb_step_y = nullptr;
	SpinBox *sb_step_x = nullptr;
	SpinBox *sb_off_y = nullptr;
	SpinBox *sb_off_x = nullptr;
	SpinBox *sb_sep_y = nullptr;
	SpinBox *sb_sep_x = nullptr;
	Panel *edit_draw = nullptr;

	VScrollBar *vscroll = nullptr;
	HScrollBar *hscroll = nullptr;

	UndoRedo *undo_redo = nullptr;

	Vector2 draw_ofs;
	float draw_zoom;
	bool updating_scroll;

	int snap_mode;
	Vector2 snap_offset;
	Vector2 snap_step;
	Vector2 snap_separation;

	Sprite2D *node_sprite_2d = nullptr;
	Sprite3D *node_sprite_3d = nullptr;
	NinePatchRect *node_ninepatch = nullptr;
	Ref<StyleBoxTexture> obj_styleBox;
	Ref<AtlasTexture> atlas_tex;

	Rect2 rect;
	Rect2 rect_prev;
	float prev_margin = 0.0f;
	int edited_margin;
	HashMap<RID, List<Rect2>> cache_map;
	List<Rect2> autoslice_cache;
	bool autoslice_is_dirty;

	bool drag;
	bool creating = false;
	Vector2 drag_from;
	int drag_index;

	Ref<ViewPanner> panner;
	void _scroll_callback(Vector2 p_scroll_vec, bool p_alt);
	void _pan_callback(Vector2 p_scroll_vec);
	void _zoom_callback(Vector2 p_scroll_vec, Vector2 p_origin, bool p_alt);

	void _set_snap_mode(int p_mode);
	void _set_snap_off_x(float p_val);
	void _set_snap_off_y(float p_val);
	void _set_snap_step_x(float p_val);
	void _set_snap_step_y(float p_val);
	void _set_snap_sep_x(float p_val);
	void _set_snap_sep_y(float p_val);
	void _zoom_on_position(float p_zoom, Point2 p_position = Point2());
	void _zoom_in();
	void _zoom_reset();
	void _zoom_out();
	void apply_rect(const Rect2 &p_rect);
	void _update_rect();
	void _update_autoslice();

	void _texture_changed();

protected:
	void _notification(int p_what);
	void _node_removed(Object *p_obj);
	static void _bind_methods();

	Vector2 snap_point(Vector2 p_target) const;

public:
	void _edit_region();
	void _region_draw();
	void _region_input(const Ref<InputEvent> &p_input);
	void _scroll_changed(float);
	bool is_stylebox();
	bool is_atlas_texture();
	bool is_ninepatch();
	Sprite2D *get_sprite_2d();
	Sprite3D *get_sprite_3d();

	void edit(Object *p_obj);
	TextureRegionEditor();
};

class TextureRegionEditorPlugin : public EditorPlugin {
	GDCLASS(TextureRegionEditorPlugin, EditorPlugin);

	bool manually_hidden;
	Button *texture_region_button = nullptr;
	TextureRegionEditor *region_editor = nullptr;

protected:
	static void _bind_methods();

	void _editor_visiblity_changed();

public:
	virtual String get_name() const override { return "TextureRegion"; }
	bool has_main_screen() const override { return false; }
	virtual void edit(Object *p_object) override;
	virtual bool handles(Object *p_object) const override;
	virtual void make_visible(bool p_visible) override;
	void set_state(const Dictionary &p_state) override;
	Dictionary get_state() const override;

	TextureRegionEditorPlugin();
};

#endif // TEXTURE_REGION_EDITOR_PLUGIN_H
