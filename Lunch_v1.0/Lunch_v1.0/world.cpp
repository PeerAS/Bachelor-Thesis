#include "pch.h"
#include "world.h"

using namespace DirectX;

World::World()
{

}

void World::Initialize(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dContext)
{
	m_d3dDevice = d3dDevice;
	m_d3dContext = d3dContext;

	m_spriteBatch = std::unique_ptr<SpriteBatch>(new SpriteBatch(d3dContext));
}


World::~World()
{

}
