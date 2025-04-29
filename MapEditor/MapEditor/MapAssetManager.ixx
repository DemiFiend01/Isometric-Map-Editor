#include <SFML/Graphics.hpp>
import "unordered_map";
#include <fstream>
#include <iostream>
export module MapAssetManager;
using BYTE = std::uint8_t; //8 bit

export class MapAssetManager
{
	sf::Texture whole_tiles_texture; //for map
	sf::Texture whole_walls_texture;
	sf::Texture whole_object_texture;
public:
	std::unordered_map<BYTE, sf::Sprite> allTileSprites;
	std::unordered_map<BYTE, sf::Sprite> allWallSprites;
	std::unordered_map<BYTE, sf::Sprite> allObjectSprites;
	int getSizeOfMap(BYTE _map_choice);
	BYTE choice;
	MapAssetManager();
	~MapAssetManager();
	void LoadTexturesForMap(std::string& filename, BYTE tiles_x, BYTE tiles_y, short texture_width, short texture_height, BYTE _choice);
	sf::Sprite getTileSprite(BYTE tile_id, BYTE _choice);
};

int MapAssetManager::getSizeOfMap(BYTE _map_choice)
{
	switch (_map_choice)
	{
	case 0:
		return allTileSprites.size();
		break;
	case 1:
		std::cout << allWallSprites.size() << std::endl;
		return allWallSprites.size();
		break;
	case 2:
		return allObjectSprites.size();
		break;
	}
	return 0;
}

MapAssetManager::MapAssetManager()
{
}

MapAssetManager::~MapAssetManager()
{
}

void MapAssetManager::LoadTexturesForMap(std::string& filename, BYTE tiles_x, BYTE tiles_y, short texture_width, short texture_height, BYTE _choice)
{
	std::unordered_map<BYTE, sf::Sprite>* kowabunga = &allTileSprites; //it needs initiation
	sf::Texture* loadFromTexture = &whole_tiles_texture;
	switch (_choice)
	{
	case 0:
		kowabunga = &allTileSprites;
	
		break;
	case 1:
		kowabunga = &allWallSprites;
		loadFromTexture = &whole_walls_texture;

		break;
	case 2:
		kowabunga = &allObjectSprites;
		loadFromTexture = &whole_object_texture;

		break;
	default:
		std::cerr << "Loading files needs a correct type specification. 0,1,2\n";
		exit(2);
	}

	if (!loadFromTexture->loadFromFile(filename))
	{
		std::cerr << "Couldn't load the tiles texture.\n";
		exit(1);
	}

	BYTE text_id = 0;
	short tile_width = texture_width/tiles_x; short tile_heigth = texture_height/tiles_y;
	std::ifstream ifile(filename);
	if (!ifile.is_open())
	{
		std::cerr << "The file for loading textures for the map is not present." << std::endl;
		exit(1);
	}
	

	sf::Sprite temp_sprite;
	for (BYTE col = 0; col < tiles_y; col++)
	{
		for (BYTE row = 0; row < tiles_x; row++)
		{
			
			temp_sprite.setTexture(*loadFromTexture);
			temp_sprite.setTextureRect(sf::IntRect(row * tile_width, col * tile_heigth, tile_width, tile_heigth));
			kowabunga->insert({ text_id,temp_sprite });
			std::cout << kowabunga->size() << "\n";
			text_id++;
		}
	}
}


sf::Sprite MapAssetManager::getTileSprite(BYTE tile_id, BYTE _choice)
{
	std::unordered_map<BYTE, sf::Sprite> kowabunga = allTileSprites; //it needs initiation
	switch (_choice)
	{
	case 0:
		kowabunga = allTileSprites;
		break;
	case 1:
		kowabunga = allWallSprites;
		break;
	case 2:
		kowabunga = allObjectSprites;
		break;
	default:
		std::cerr << "Loading files needs a correct type specification. 0,1,2\n";
	}
	if (kowabunga.find(tile_id) != kowabunga.end())
	{
		sf::Sprite sprite = kowabunga[tile_id];
		std::cout << std::to_string(kowabunga[tile_id].getTextureRect().height);
		//std::cout << "got the texture" << tile_id << std::endl;
		float scale_x = 40.f / sprite.getTextureRect().width;
		float scale_y = 20.f / sprite.getTextureRect().height;
		if (_choice == 1)
			scale_y = 80.f / sprite.getTextureRect().height;
		else if (_choice == 2)
			scale_y = 40.f / sprite.getTextureRect().height;
		//float scale_y = _choice == 0 ? 20.f / sprite.getTextureRect().height : 40.f / sprite.getTextureRect().height;
		//change the scaling for walls now, keep it different to objectrs
		
		std::cout << std::to_string(kowabunga[tile_id].getTextureRect().height)<<std::endl<<std::to_string(scale_y)<<std::endl;
		sprite.setScale({ scale_x,scale_y }); //resize the sprite so that it is 40 by 20
		return sprite; //return a copy
	}
	else
	{
		std::cerr << "No tile texture with this id\n";
		exit(2);
	}
}