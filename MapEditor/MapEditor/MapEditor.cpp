#include "MapEditor.h"

//to do:
// when map editing press escape to go back to menu, the map should be cleaned then
// when saving the map, update the mapbuttons
//
void MapEditor::run()
{
	sf::Event ev;
	while (win->isOpen()) //the main game loop
	{
		while (win->pollEvent(ev))
		{
			if (prog_state == Program_state::MapEditing)
				handleInputsMapEditing(ev);
			else
				handleInputsMenuAndSelect(ev);
		}
		float delta_time = clock.restart().asSeconds();
		update(delta_time);
		win->clear();
		if (prog_state == Program_state::MapEditing)
			renderMap();
		else if (prog_state == Program_state::Menu)
			renderMenu();
		else
			renderMapSelect();
		win->display();
	}
}

void MapEditor::handleInputsMapEditing(sf::Event _ev)
{
	sf::Mouse mouse_map;
	sf::Vector2i mouse_pos = mouse_map.getPosition(*win);
	mouse_pos.x = win->mapPixelToCoords(mouse_pos).x;
	mouse_pos.y = win->mapPixelToCoords(mouse_pos).y;

	std::pair<BYTE, BYTE> temp;
	switch (_ev.type)
	{
	case sf::Event::Closed:
		win->close();
		break;
	case sf::Event::MouseWheelScrolled:
		if (_ev.mouseWheelScroll.delta > 0)
			win_view.zoom(1.f / view_zoom);
		else
			win_view.zoom(view_zoom);
		break;
	case sf::Event::MouseButtonPressed:
		if (_ev.mouseButton.button == sf::Mouse::Left)
		{
			std::cout << "left\n";
			std::cout << "CLicked\n";
			switch (working_type)
			{
			case FloorTiles:
				this->placeTileVer2(mouse_pos, 20, 40, 0);
				std::cout << "FloorTiles\n";
				break;
			case Walls:
				this->placeTileVer2(mouse_pos, 20, 40, 5);
				std::cout << "Walls\n";
				break;
			case Transition:
				this->placeTransitionStates(mouse_pos, 20, 40);
				std::cout << "transitions\n";
				break;
			case Extra:
				this->placeTileVer2(mouse_pos, 20, 40, 6); //i dont know how to implement just 4 extra interractable objects so they wont exist yet
				std::cout << "Extra\n";
				break;
			}
		}
		if (_ev.mouseButton.button == sf::Mouse::Middle)
			win_view.reset(sf::FloatRect(0.f, 0.f, 921.f, 512.f));
		break;
	case sf::Event::KeyPressed:
		if (_ev.key.code == sf::Keyboard::W || _ev.key.code == sf::Keyboard::Up)
			win_view.move(0.f, -10.f);
		if (_ev.key.code == sf::Keyboard::S || _ev.key.code == sf::Keyboard::Down)
			win_view.move(0, 10.f);
		if (_ev.key.code == sf::Keyboard::A || _ev.key.code == sf::Keyboard::Left)
			win_view.move(-10.f, 0.f);
		if (_ev.key.code == sf::Keyboard::D || _ev.key.code == sf::Keyboard::Right)
			win_view.move(10.f, 0.f);
		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::S) && _ev.key.code == sf::Keyboard::LControl) || (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && _ev.key.code == sf::Keyboard::S))
		{
			std::cout << "pressed ctrl+s\n";
			this->saveIntoFile();
		}
		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::C) && _ev.key.code == sf::Keyboard::R) || (sf::Keyboard::isKeyPressed(sf::Keyboard::R) && _ev.key.code == sf::Keyboard::C))
		{
			std::cout << "Clear the map\n"; // clear the map
			this->clearTheMap();

		}
		if (_ev.key.code == sf::Keyboard::Q)
		{
			win_view.reset(sf::FloatRect(0.f, 0.f, 921.f, 512.f));
			initMapLoadingRender(); //loading maps from scratch if we've saved new
			prog_state = Program_state::Menu;
		}
		switch (working_type)
		{
		case FloorTiles:
			switch (_ev.key.code)
			{
			case sf::Keyboard::E:
				working_type = Extra; //change the working type
				std::cout << "Changing to extra from FloorTiles\n";
				break;
			case sf::Keyboard::Y:
				working_type = Walls;
				std::cout << "Changing to walls from FloorTiles\n";
				break;
			case sf::Keyboard::T:
				working_type = Transition;
				std::cout << "Changing to transition from FloorTiles\n";
				break;
			}
			break;
		case Walls:
			switch (_ev.key.code)
			{
			case sf::Keyboard::E:
				working_type = Extra;
				std::cout << "Changing to extra from walls\n";
				break;
			case sf::Keyboard::R:
				working_type = FloorTiles;
				std::cout << "Changing to floor from walls\n";
				break;
			case sf::Keyboard::T:
				working_type = Transition;
				std::cout << "Changing to transition from walls\n";
				break;
			}
		case Transition:
			switch (_ev.key.code)
			{
			case sf::Keyboard::E:
				working_type = Extra;
				std::cout << "Changing to extra from transition\n";
				break;
			case sf::Keyboard::Y:
				working_type = Walls;
				std::cout << "Changing to walls from transition\n";
				break;
			case sf::Keyboard::R:
				working_type = FloorTiles;
				std::cout << "Changing to FloorTiles from transition\n";
				break;
			}
			break;
		case Extra:
			switch (_ev.key.code)
			{
			case sf::Keyboard::T:
				working_type = Transition;
				std::cout << "Changing to transition from extra\n";
				break;
			case sf::Keyboard::Y:
				working_type = Walls;
				std::cout << "Changing to walls from extra\n";
				break;
			case sf::Keyboard::R:
				working_type = FloorTiles;
				std::cout << "Changing to FloorTiles from extra\n";
				break;
			}
			break;
		}
		
	}
}

void MapEditor::handleInputsMenuAndSelect(sf::Event _ev)
{
	sf::Mouse temp_mouse;
	sf::Vector2f mouse_pos_temp(sf::Mouse::getPosition(*win).x, sf::Mouse::getPosition(*win).y);
	switch (_ev.type)
	{
	case sf::Event::Closed:
		win->close();
		break;
	case sf::Event::MouseButtonPressed:
		
		if (prog_state == Program_state::Menu)
		{
			if (MenuButtons[0].first.getGlobalBounds().contains(mouse_pos_temp))
			{
				std::cout << "CLICKED\n";
				prog_state = Program_state::MapEditing;
			}
			if (MenuButtons[1].first.getGlobalBounds().contains(mouse_pos_temp))
			{
				std::cout << "Loading a map\n";
				prog_state = Program_state::MapSelect;
			}
			if (MenuButtons[2].first.getGlobalBounds().contains(mouse_pos_temp))
			{
				std::cout << "Quitting\n";
				win->close();
			}
		}
		else
		{
			if (map_sel_load_button.first.getGlobalBounds().contains(mouse_pos_temp))
			{
				std::cout << "Loading the map\n";
				if (loaded_map == "")
				{
					std::cout << "Load a map firsthand!\n";
					info_sel_map.second.setString("Choose a map!");
					show_info_chosen_map = true;
				}
				else
				{
					info_sel_map.second.setString("Loading map:\n"+loaded_map);
					show_info_chosen_map = true;
					trans_map_loaded = true;
				}
				//load the map
			}
			for (int i = 0; i < howManyBoxesToShow; i++)
			{
				int idx = i + scrollOffset;
				if (idx >= 0 && idx < MapButtons.size())
				{
					auto& map_button = MapButtons[idx];
					if (map_button.first.first.getGlobalBounds().contains(mouse_pos_temp))
					{
						loaded_map = map_button.second;
						std::cout << "Selected: " << loaded_map << std::endl; 
						info_sel_map.second.setString("Selected map:\n" + loaded_map);
						show_info_chosen_map = true;
						//
						break;
					}

				}
				
			}
			
			//clicking on a particular map to load it
		}
		
		//selected one of the buttons
		break;
	case sf::Event::KeyPressed:
		if (prog_state == Program_state::MapSelect)
		{
			if (_ev.key.code == sf::Keyboard::W)
			{
				if (scrollOffset > 0)
					scrollOffset--;
			}
			if (_ev.key.code == sf::Keyboard::S)
			{
				if (scrollOffset + howManyBoxesToShow < MapButtons.size())
					scrollOffset++;
			}
			if (_ev.key.code == sf::Keyboard::Q)
				prog_state = Program_state::Menu;
		}
		break;
	}
}

void MapEditor::update(float _delta_time)
{
	
	if (prog_state == Program_state::MapSelect)
	{
		for (int i = 0; i < MapButtons.size(); i++)
		{
			int visIdx = i - scrollOffset;
			if (visIdx >= 0 && visIdx < howManyBoxesToShow)
			{
				this->MapButtons[i].first.first.setPosition({ 20.f,10.f + visIdx * 50.f });
				this->MapButtons[i].first.second.setPosition({ 25.f,12.f + visIdx * 50.f });
			}
		}
		if (show_info_chosen_map)
		{
			info_sel_map_timer += 0.1;
			if (info_sel_map_timer > 5)
			{
				show_info_chosen_map = false;
				info_sel_map_timer = 0;
			}
		}
		else if (trans_map_loaded)
		{
			//transition to map editing but before load the map
			std::cout << "Transitioning\n";
			trans_map_loaded = false;
			readFromFile(loaded_map);
			prog_state = Program_state::MapEditing;
		}
	}
	this->win->setView(win_view);
}

void MapEditor::renderMenu()
{
	for (int i = 0; i < MenuButtons.size(); i++)
	{
		win->draw(MenuButtons[i].first);
		win->draw(MenuButtons[i].second);
	}
}

void MapEditor::renderMap()
{

	for (const auto& sprite : sprites_from_arrays)
	{
		win->draw(sprite);
	}
	for (const auto& sprite : transition_states)
		win->draw(sprite);
}

void MapEditor::renderMapSelect()
{
	
	for (int i = 0; i < howManyBoxesToShow; i++)
	{
		int idx = i + scrollOffset;
		if (idx >= 0 && idx < MapButtons.size()) {
			win->draw(MapButtons[idx].first.first);
			win->draw(MapButtons[idx].first.second);
		}
	}
	if (show_info_chosen_map) {
		win->draw(info_sel_map.first);
		win->draw(info_sel_map.second);
	}
	win->draw(map_sel_load_button.first);
	win->draw(map_sel_load_button.second);
}

void MapEditor::initMapLoadingRender()
{
	MapButtons.clear();
	sf::RectangleShape temp_rectangle;
	temp_rectangle.setFillColor(sf::Color::Green);
	temp_rectangle.setSize({ 200.f,40.f });
	sf::Text temp_text;
	temp_text.setCharacterSize(12);
	temp_text.setFillColor(sf::Color::White);
	temp_text.setFont(font); //must make sure it is loaded beforehand
	std::filesystem::path map_folder = std::filesystem::current_path() / "maps";
	int temp = 0;
	if (!std::filesystem::exists(map_folder))
	{
		std::cout << "No maps available.\n";
		return;
	}
	std::regex file_index_pattern("Map_(\\d+)\\.txt"); //match just the numbers
	std::vector<std::pair<int, std::string>> file_names_sorted;
	for (auto const& dir : std::filesystem::directory_iterator(map_folder))
	{
		std::filesystem::path temp_path = dir;
		temp_path = temp_path.filename();
		std::string s = temp_path.string();
		std::smatch found_index;
		if (std::regex_search(s, found_index, file_index_pattern))
		{
			std::cout << "Found a correct map\n";
			std::string temp_file = temp_path.string();
			int extracted_idx = std::stoi(found_index[1].str());
			file_names_sorted.push_back(std::make_pair(extracted_idx, temp_file));
			std::cout << temp_path << std::endl;
		}
		std::cout << dir.path() << std::endl;
	}
	std::ranges::sort(file_names_sorted, [](const auto& a, const auto& b) { return a.first < b.first; });
	for (auto i : file_names_sorted)
	{
		temp_text.setString(i.second);
		MapButtons.push_back({ {temp_rectangle, temp_text}, i.second });
	}
	howManyBoxesToShow = std::min(static_cast<int>(MapButtons.size()), 10);

	info_sel_map.first.setSize({ 200,40 });
	info_sel_map.first.setFillColor(sf::Color::Green);
	info_sel_map.first.setPosition({ 400.f,200.f });
	temp_text.setString("Loaded map.");
	info_sel_map.second = temp_text;
	info_sel_map.second.setPosition({ 405.f,205.f });

	map_sel_load_button.first.setSize({ 200,40 });
	map_sel_load_button.first.setFillColor(sf::Color::Green);
	map_sel_load_button.first.setPosition({ 700.f,10.f });
	temp_text.setString("Modify map");
	map_sel_load_button.second = temp_text;
	map_sel_load_button.second.setPosition({ 705.f,12.f });
}
void MapEditor::initRender()
{
	//init menu
	sf::RectangleShape temp_rectangle;
	temp_rectangle.setSize({ 200.f,70.f });
	temp_rectangle.setFillColor(sf::Color::Green);
	std::string button_texts[3] = { "Create New Map", "Modify Map", "Quit" };
	sf::Text temp_text;
	temp_text.setCharacterSize(12);
	temp_text.setFillColor(sf::Color::White);
	temp_text.setFont(font); //must make sure it is loaded beforehand
	for (int i = 0; i < 3; i++)
	{
		temp_text.setString(button_texts[i]);
		this->MenuButtons.push_back(std::make_pair(temp_rectangle,temp_text));
		MenuButtons[i].first.setPosition({ 100.f,150.f + (80 * i) });
		MenuButtons[i].second.setPosition({ 110.f,160.f + ( 80 * i) });
	}
	//init select
	this->initMapLoadingRender();

}

void MapEditor::initNewMap()
{
	map_tile temp_tile = { 0,0,000 };
	map_tile temp_walls = { 0, 0, 500 };//itll be hopefully null?
	map_tile temp_object = { 0, 0, 600 };
	sprites_from_arrays.clear(); //this hopefully allows us to call this function more than once
	
	for (BYTE i = 0; i < size_of_array; i++)
		for (BYTE j = 0; j < size_of_array; j++)
		{
			temp_tile.tile_x = i; temp_tile.tile_y = j;
			tiles_array[i][j] = temp_tile; //000 means its default, not walkable, not not walkable either, nothing, bare, will not be transfered to the real map
			walls_array[i][j] = temp_walls;
			objects_array[i][j] = temp_object;
			//sf::Vector2f screen_pos(origin_pos.x + ((j-i) * tile_width / 2.f),origin_pos.y + ((i + j) * tile_height / 2.f));

			sf::Vector2f screen_pos(origin_pos.x + ((i-j) * (tile_width / 2.f)), origin_pos.y + ((i + j) * (tile_height / 2.f)));

			sf::Sprite temp_sprite = texture_manager.getTileSprite(tiles_array[i][j].info % 100, 0);
			temp_sprite.setPosition(screen_pos);
			this->sprites_from_arrays.push_back(temp_sprite);

			sf::Sprite temp_sprite2 = texture_manager.getTileSprite(walls_array[i][j].info % 100, 1);
			temp_sprite2.setPosition(screen_pos - sf::Vector2f{ 0,60 });
			this->sprites_from_arrays.push_back(temp_sprite2);

			sf::Sprite temp_sprite3 = texture_manager.getTileSprite(objects_array[i][j].info % 100, 2);
			temp_sprite3.setPosition(screen_pos - sf::Vector2f{ 0,60 });
			this->sprites_from_arrays.push_back(temp_sprite3);
		}
}

void MapEditor::initLoading()
{
	//std::string s = "Assets/iso_rev.png";
	std::string s = "Assets/isometric_tiles_floor.png";
	//texture_manager.LoadTexturesForMap(s, 3, 2, 120, 40,0);//0 is for the tiles
	texture_manager.LoadTexturesForMap(s, 5, 2, 200, 40, 0);//0 is for the tiles
	//s = "Assets/walls_wip.png";
	s = "Assets/isometric_tiles_walls.png";
	texture_manager.LoadTexturesForMap(s, 6, 3, 240, 240, 1);
	s = "Assets/objects_wip.png";
	texture_manager.LoadTexturesForMap(s, 6, 1, 240, 40, 2);
	s = "Assets/Font/SixtyfourConvergence-Regular.ttf";
	if (font.loadFromFile(s))
		std::cout << "Loaded the font.\n";


}

void MapEditor::initWindow()
{
	sf::VideoMode temp_video_mode;
	temp_video_mode.height = 512; temp_video_mode.width = 921;

	this->win = new sf::RenderWindow(temp_video_mode, "Map editor", sf::Style::Default);
	win->setFramerateLimit(60);

	win_view.reset(sf::FloatRect(0.f, 0.f, 921.f, 512.f));
	win->setView(win_view); //need to do -> when working with pointers


}

void MapEditor::initVariables()
{
	menu_choice = 0;
	origin_pos.x = 400;
	origin_pos.y = 20;
	
	//i dont even know yet
}

void MapEditor::saveIntoFile() //no string required actually
{
	std::filesystem::path map_folder = std::filesystem::current_path() / "maps";
	int temp = 0;
	int max_index = -1;
	if (!std::filesystem::exists(map_folder))
	{
		std::filesystem::create_directory(map_folder);
	}
	//std::regex file_index_pattern(R"(\d+)");
	std::regex file_index_pattern("Map_(\\d+)\\.txt"); //match just the numbers
	for (auto const& dir : std::filesystem::directory_iterator(map_folder))
	{	
		std::filesystem::path temp_path = dir;
		temp_path = temp_path.filename();
		std::string s = temp_path.string();
		std::smatch found_index;
		if (std::regex_search(s, found_index, file_index_pattern))
		{
			std::cout << "Found numbers in the filename!\n";
			std::string temp_file = found_index[1].str();
			int index = std::stoi(temp_file);
			if (index > max_index && index < 100)
				max_index = index;
			std::cout << max_index << std::endl;
		}
		std::cout << dir.path() << std::endl;
	} //find the path with the highest number of the map
	temp = max_index + 1;
	std::string file_name = "Map_" + std::to_string(temp) + ".txt";
	std::filesystem::path map_location = map_folder / (file_name);

	std::ofstream ofile(map_location);
	if (!ofile.is_open())
	{
		std::cerr << "Couldn't output the map for some reason.\n";
	}
	for (BYTE i = 0; i < size_of_array; i++)
		for (BYTE j = 0; j < size_of_array; j++)
		{
			short temp_info;
			temp_info = tiles_array[i][j].info;
			if (temp_info % 100)
				output_map.insert(tiles_array[i][j]);
			//do i even need this output map
			temp_info = walls_array[i][j].info;
			if (temp_info % 100)
				output_map.insert(walls_array[i][j]);
			//no x y theyre all 0
			temp_info = objects_array[i][j].info;
			if (temp_info % 100)
				output_map.insert(objects_array[i][j]);
		}

	for (auto i : output_map)
	{
		ofile <<std::setw(3)<<std::setfill('0')<< +(int)i.tile_x << ";" << std::setw(3) << std::setfill('0') << +(int)i.tile_y << "_" << std::setw(3) << std::setfill('0') << i.info << "\t";
	}


	ofile.close();
}

void MapEditor::readFromFile(std::string file_name)
{
	//load the map from the file
	std::filesystem::path map_folder = std::filesystem::current_path() / "maps";
	std::filesystem::path map_location = map_folder / (file_name);
	if (!std::filesystem::exists(map_folder))
	{
		std::cerr << "Cannot read from file, because the map folder does not exist";
		return;
	}
	std::vector<map_tile> temp_from_file;

	std::ifstream i_file;
	i_file.open(map_location);
	if (!i_file.is_open())
	{
		std::cerr << "Cannot open the file.\n";
		return;
	}
	clearTheMap();
	std::string s;
	std::regex tile_pattern(R"((\d{3});(\d{3})_(\d{3}))");
	if (std::getline(i_file, s))
	{
		std::stringstream ss(s);
		std::string tile;
		while(std::getline(ss,tile, '\t'))
		{
			
			std::smatch found_tile;
			if (std::regex_search(tile, found_tile, tile_pattern))
			{
				std::cout << "Found tile!\n" << found_tile[0].str() << std::endl;
				int x = std::stoi(found_tile[1]); //need to make it so its filled with zeroes
				int y = std::stoi(found_tile[2]);
				short in = std::stoi(found_tile[3]);
				map_tile temp_tile{ x,y, in };
				temp_from_file.push_back(temp_tile);
				//update the sprites
			}
		}
	}
	
	int index_offset = 0;
	int choice;
	sf::RectangleShape temp_rect_sh;
	temp_rect_sh.setSize({ 40.f,20.f });
	temp_rect_sh.setOutlineThickness({ 1.f });
	for (auto& i : temp_from_file)
	{
		choice = i.info / 100;
		sf::Vector2f screen_pos(origin_pos.x + ((i.tile_x-i.tile_y) * (tile_width / 2.f)), origin_pos.y + ((i.tile_x + i.tile_y) * (tile_height / 2.f)));
		if (choice == 0)
		{
			tiles_array[i.tile_x][i.tile_y] = i;
			index_offset = 0;
		}
		else if (choice < 5) //transition states
		{
			
			temp_rect_sh.setPosition(screen_pos);
			
			temp_rect_sh.setFillColor(sf::Color::Transparent);
			auto it = std::remove_if(transition_states.begin(), transition_states.end(), [&](const sf::RectangleShape& rect)
				{return rect.getPosition() == screen_pos;
				}
			);
			switch (choice)
			{
			case 1:
				temp_rect_sh.setOutlineColor(sf::Color::Red);
				transition_states.push_back(temp_rect_sh);
				break;
			case 2:
				temp_rect_sh.setOutlineColor(sf::Color::Blue);
				transition_states.push_back(temp_rect_sh);
				break;
			case 3:
				temp_rect_sh.setOutlineColor(sf::Color::Green);
				transition_states.push_back(temp_rect_sh);
				break;
			case 4:
				temp_rect_sh.setOutlineColor(sf::Color::Magenta);
				transition_states.push_back(temp_rect_sh);
				break;
			}
		}
		else if (choice == 5)
		{
			walls_array[i.tile_x][i.tile_y] = i;
			index_offset = 1;
		}
		else
		{
			objects_array[i.tile_x][i.tile_y] = i;
			index_offset = 2;
		}
		//swap the corresponding sprite
		int index = (i.tile_x * size_of_array + i.tile_y) * 3 + index_offset; 
		if (index < sprites_from_arrays.size())
		{
			sprites_from_arrays[index] = texture_manager.getTileSprite(i.info%100,index_offset);
			if (index_offset)
				screen_pos -= sf::Vector2f{ 0,60 };
			sprites_from_arrays[index].setPosition(screen_pos);
		}
	}
	i_file.close();
	return;
}


void MapEditor::placeTileVer2(sf::Vector2i mouse_pos, BYTE tile_height, BYTE tile_width, short _info)
{
	std::pair<BYTE, BYTE> temp_pos = screenToIsoMap(mouse_pos, tile_height, tile_width);
	map_tile temp_tile = { temp_pos.first,temp_pos.second,_info };
	short choice = _info;
	short index_offset = 0;
	short texture_choice = 0;
	short temp_texture = 0;
	if (choice >= 0 && choice < 5)
	{
		temp_texture = tiles_array[temp_pos.first][temp_pos.second].info % 100 + 1;
		if (temp_texture >= texture_manager.getSizeOfMap(texture_choice))
			temp_texture = 0; //loop back
		
		short calculated_new_info = _info * 100 + temp_texture;
		tiles_array[temp_pos.first][temp_pos.second] = { temp_pos.first, temp_pos.second, calculated_new_info };
	}
	else if (choice == 5)
	{
		index_offset = 1;
		texture_choice = 1;
		temp_texture = walls_array[temp_pos.first][temp_pos.second].info % 100 + 1;
		std::cout << "walls temp texture " << temp_texture << std::endl;
		if (temp_texture >= texture_manager.getSizeOfMap(texture_choice))
			temp_texture = 0; //loop back
		std::cout << "walls temp texture2 " << temp_texture << std::endl;
		short calculated_new_info = _info * 100 + temp_texture;
		walls_array[temp_pos.first][temp_pos.second] = { temp_pos.first, temp_pos.second, calculated_new_info };
		
		std::cout << "walls new info: " << walls_array[temp_pos.first][temp_pos.second].info << std::endl;
	}
	else
	{
		index_offset = 2;
		texture_choice = 2;
		temp_texture = objects_array[temp_pos.first][temp_pos.second].info % 100 + 1;
		if (temp_texture >= texture_manager.getSizeOfMap(texture_choice))
			temp_texture = 0; //loop back

		short calculated_new_info = _info * 100 + temp_texture;
		objects_array[temp_pos.first][temp_pos.second] = { temp_pos.first, temp_pos.second, calculated_new_info };
		
	}
	int index = (temp_pos.first * size_of_array + temp_pos.second) * 3 + index_offset; //for now itll update just the tiles hopefully
	if (index < sprites_from_arrays.size())
	{
		sprites_from_arrays[index] = texture_manager.getTileSprite(temp_texture % 100,texture_choice);
		sf::Vector2f screen_position;
		if (choice >= 5) //fix for objects too
		{
			screen_position = sf::Vector2f((float)(origin_pos.x) + (((float)(temp_pos.first) - temp_pos.second) * (float)(tile_width) / 2.f),
				origin_pos.y + (temp_pos.first + temp_pos.second) * (tile_height) / 2.f - 60.f);
		}
		else
		{
			screen_position = sf::Vector2f(origin_pos.x + ((temp_pos.first - temp_pos.second) * tile_width / 2.f),
				origin_pos.y + (temp_pos.first + temp_pos.second) * tile_height / 2.f);
		}
		
		sprites_from_arrays[index].setPosition(screen_position);
	}
}

void MapEditor::placeTransitionStates(sf::Vector2i mouse_pos, BYTE tile_height, BYTE tile_width)
{
	std::pair<BYTE, BYTE> temp_pos = screenToIsoMap(mouse_pos, tile_height, tile_width);
	short choice = 0;
	//map_tile temp_tile = { temp_pos.first,temp_pos.second,choice };//just temporary
	short temp_choice = tiles_array[temp_pos.first][temp_pos.second].info;
	temp_choice += temp_choice / 100 >= 4 ? -400 : 100;
	this->tiles_array[temp_pos.first][temp_pos.second].info = temp_choice; 

	choice = temp_choice / 100;
	std::cout << "choice " << choice << std::endl;

	sf::RectangleShape temp_rect_sh;
	sf::Vector2f screen_position = sf::Vector2f(origin_pos.x + ((temp_pos.first - temp_pos.second) * tile_width / 2.f),
		origin_pos.y + (temp_pos.first + temp_pos.second) * tile_height / 2.f);
	temp_rect_sh.setSize({ 40.f,20.f });
	temp_rect_sh.setPosition(screen_position); 
	temp_rect_sh.setOutlineThickness({ 1.f });
	temp_rect_sh.setFillColor(sf::Color::Transparent);
	auto it = std::remove_if(transition_states.begin(), transition_states.end(), [&](const sf::RectangleShape& rect)
		{return rect.getPosition() == screen_position;
		}
	);
	switch (choice)
	{
	case 0:
		transition_states.erase(it, transition_states.end());
		break;
	case 1:
		temp_rect_sh.setOutlineColor(sf::Color::Red);
		transition_states.push_back(temp_rect_sh);
		break;
	case 2:
		temp_rect_sh.setOutlineColor(sf::Color::Blue);
		transition_states.push_back(temp_rect_sh);
		break;
	case 3:
		temp_rect_sh.setOutlineColor(sf::Color::Green);
		transition_states.push_back(temp_rect_sh);
		break;
	case 4:
		temp_rect_sh.setOutlineColor(sf::Color::Magenta);
		transition_states.push_back(temp_rect_sh);
		break;
	}
}


std::pair<BYTE, BYTE> MapEditor::screenToIsoMap(sf::Vector2i mouse_pos, BYTE tile_height, BYTE tile_width)
{
	float mouse_x = static_cast<float>(mouse_pos.x - origin_pos.x);
	float mouse_y = static_cast<float>(mouse_pos.y - origin_pos.y);

	std::cout << mouse_pos.x << "  " << mouse_x << std::endl;
	std::cout << mouse_pos.y << "  " << mouse_y << std::endl;

	float half_tile_width = static_cast<float>(tile_width / 2.f);
	float half_tile_height = static_cast<float>(tile_height / 2.f);

	float iso_x = ((mouse_x / half_tile_width) + (mouse_y / half_tile_height)) / 2;
	float iso_y = ((mouse_y/ half_tile_height) - (mouse_x/ half_tile_width)) / 2;

	std::cout << "float:" << iso_x << "  " << iso_y << std::endl;

	int tile_x = static_cast<int>(iso_x-0.5f);
	int tile_y = static_cast<int>(iso_y+0.5f);
	std::cout << "tilex tiley :" << tile_x << "  " << tile_y << std::endl;

	if (tile_x < 0 || tile_x >= size_of_array || tile_y < 0 || tile_y >= size_of_array)
		return { 0,0 }; //for now i suppose, should change this function into void probably? or the other one i guess
	std::pair<BYTE, BYTE> result(static_cast<BYTE>(tile_x),static_cast<BYTE>(tile_y));
	return result;
}

void MapEditor::removeTile(BYTE _x, BYTE _y, short _info)
{
	if (!output_map.erase({ _x,_y,_info }))
	{
		std::cerr << "Couldn't erase the tile from the output map.\n";
		exit(2);
	}
}

void MapEditor::clearTheMap()
{
	//looks a bit silly
	initNewMap();
	output_map.clear(); //used solely for outputting to file
	transition_states.clear();
}

MapEditor::MapEditor()
{
	initWindow();
	initVariables();
	initLoading();
	initRender();
	initNewMap();
	
}
