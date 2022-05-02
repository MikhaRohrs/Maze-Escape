#pragma once

#include <SFML/Graphics.hpp>
#include <maths.h>
#include <memory>
#include <string>
#include <vector>
#include <map>

#define ls LevelSystem

class LevelSystem {
public:
  static void loadLevelFile(const std::string&, float tileSize = 100.0f);
  static void unload();
  static void render(sf::RenderWindow& window, sf::Vector2f playerPos);

  typedef unsigned char Tile;

  enum TILES {
    EMPTY = ' ',
    START = 's',
    END = 'e',
    WALL = 'w',
    ENEMY = 'n',
    WAYPOINT = '+',
    WEAPON = 'g',
    POWERUP_SPEED = '1',
    POWERUP_MAP = '2',
    POWERUP_AMMO = '3'
  };

  static Tile getTile(sf::Vector2ul);

  static Tile getTileAt(sf::Vector2f);

  static bool isOnGrid(sf::Vector2f);

  static size_t getWidth();

  static size_t getHeight();

  static sf::Vector2f getTilePosition(sf::Vector2ul);

  static std::vector<sf::Vector2ul> findTiles(Tile);

  static sf::Color getColor(Tile t);

  static void setColor(Tile t, sf::Color c);

  static void SetTexture(Tile t, sf::Texture tex);

  static sf::Texture GetTexture(Tile t);

  static void setOffset(const sf::Vector2f& _offset);

  static const sf::Vector2f& getOffset();

  static float getTileSize();

protected:
  static std::unique_ptr<Tile[]> _tiles;
  static size_t _width;
  static size_t _height;
  static sf::Vector2f _offset;

  static sf::Texture wallTex;
  static sf::Texture exitTex;
  static sf::Texture transparentTex;
  static sf::Texture spriteSheet;

  static std::vector<std::unique_ptr<sf::RectangleShape>> _sprites;

  static void buildSprites(bool optimise = false);

  static float _tileSize; // for rendering
  static std::map<Tile, sf::Color> _colours;
  static std::map <Tile, sf::Texture> textures;

private:
  LevelSystem() = delete;

  ~LevelSystem() = delete;
};
