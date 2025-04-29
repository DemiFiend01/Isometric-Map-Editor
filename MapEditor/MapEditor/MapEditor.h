#pragma once
import MapAssetManager;
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream> //for seperating by tab
#include <set>
#include <algorithm>
#include <regex>
#include <ranges>

using BYTE = std::uint8_t; //8 bit

struct map_tile
{
	BYTE tile_x; BYTE tile_y;
	short info; // 123 1 is for the type of tile 23 are for textures, its made so that itll be easier to sort hopefully
	
	//map_tile() : tile_x(0), tile_y(0), info(0) {} //default constructor
	//map_tile(BYTE _x, BYTE _y, short _info=0) : tile_x(_x), tile_y(_y), info(_info) {} //the info is initialised as 000 basically
	
	bool operator<(const map_tile& other) const {
		return std::tie(tile_x, tile_y, info) < std::tie(other.tile_x, other.tile_y, other.info);
	}
};

enum Tile_selection {
	FloorTiles,
	Walls,
	Transition,
	Extra
}; 

enum Program_state {
	Menu,
	MapSelect,
	MapEditing, //doesnt matter if from scratch or not
};

class MapEditor
{
private:
	//there will be three files
	// 1 floor tiles 40 by 20
	// 2 wall tiles 40 by 40
	// 3 extra objects ? 40 by 40 maybe
	Tile_selection working_type = FloorTiles;
	Program_state prog_state = Menu; //at first
	MapAssetManager texture_manager;
	sf::Clock clock;

	const float view_move_speed = 5.f;
	const float view_zoom = 1.1f;
	sf::View win_view;

	bool show_info_chosen_map = false;
	sf::Texture tile_texture;
	static const BYTE size_of_array = 24; //needs to be static and const, otherwise it wont let me assign size
	BYTE tile_height = 20;
	BYTE tile_width = tile_height << 1; //isometric tile standard 40
	BYTE walls_heigth = tile_width + 40; //hope it makes sense 40

	short scrollOffset = 0; //dont know if its just for skills or for items too
	int howManyBoxesToShow;

	
	map_tile tiles_array[size_of_array][size_of_array]; //the map that we will be working on the entire program basically 
	map_tile walls_array[size_of_array][size_of_array]; //all will be empty for now (as in blank textures)
	map_tile objects_array[size_of_array][size_of_array];
	std::vector<sf::Sprite> sprites_from_arrays; //one vector for now
	std::vector<sf::RectangleShape> transition_states;
	std::set<map_tile> output_map; //this will be filled up
	sf::RenderWindow* win;
	sf::Vector2i origin_pos;

	BYTE menu_choice;
	
	std::vector<std::pair<sf::RectangleShape, sf::Text>> MenuButtons;
	std::vector < std::pair<std::pair<sf::RectangleShape, sf::Text>, std::string>> MapButtons;
	std::pair<sf::RectangleShape, sf::Text> info_sel_map;
	std::pair<sf::RectangleShape, sf::Text> map_sel_load_button;
	std::string loaded_map = "";
	float info_sel_map_timer = 0;
	bool trans_map_loaded = false;
	sf::Font font;
	sf::Text text;
	
	void handleInputsMapEditing(sf::Event _ev);
	void handleInputsMenuAndSelect(sf::Event _ev);
	void update(float _delta_time);
	void renderMenu();
	void renderMap();
	void renderMapSelect();
	void initRender();
	void initMapLoadingRender();
	void initNewMap();
	void initLoading();
	void initWindow();
	void initVariables();
	void saveIntoFile();
	void readFromFile(std::string file_name);
	void placeTileVer2(sf::Vector2i mouse_pos, BYTE tile_height, BYTE tile_width, short _info);
	void placeTransitionStates(sf::Vector2i mouse_pos, BYTE tile_height, BYTE tile_width);//_choice - 0 no transition state, 1 - non walkable, 2 - transition state down, 3 - transition state parallel, 4 - transition state up
	std::pair<BYTE,BYTE> screenToIsoMap(sf::Vector2i mouse_pos, BYTE tile_height, BYTE tile_width);
	void removeTile(BYTE _x, BYTE _y, short _info);
	void clearTheMap();
public:
	MapEditor();
	~MapEditor() { delete this->win; };
	void run();
};