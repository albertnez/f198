#include "particle_node.h"
#include "data_tables.h"
#include "resource_holder.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <algorithm>


namespace {
const std::vector<ParticleData> Table = initialize_particle_data();
}

ParticleNode::ParticleNode(Particle::Type type, const TextureHolder& textures)
    : SceneNode(),
      m_particles(),
      m_texture(textures.get(Textures::Particle)),
      m_type(type),
      m_vertex_array(sf::Quads),
      m_needs_vertex_update(true) {
}

void ParticleNode::add_particle(sf::Vector2f position) {
	Particle particle;
	particle.position = position;
	particle.color = Table[m_type].color;
	particle.lifetime = Table[m_type].lifetime;

	m_particles.push_back(particle);
}

Particle::Type ParticleNode::get_particle_type() const {
	return m_type;
}

unsigned int ParticleNode::get_category() const {
	return Category::ParticleSystem;	
}

void ParticleNode::update_current(sf::Time dt, CommandQueue&) {
	// Remove expired particles at beginning
	while (!m_particles.empty() && m_particles.front().lifetime <= sf::Time::Zero)
		m_particles.pop_front();

	// Decrease lifetime of existing particles
  for (Particle& particle : m_particles)
		particle.lifetime -= dt;

	m_needs_vertex_update = true;
}

void ParticleNode::draw_current(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (m_needs_vertex_update)
	{
		compute_vertices();
		m_needs_vertex_update = false;
	}

	// Apply particle texture
	states.texture = &m_texture;
	
	// Draw vertices
	target.draw(m_vertex_array, states);
}

void ParticleNode::add_vertex(float worldX, float worldY, float texCoordX, float texCoordY, const sf::Color& color) const
{
	sf::Vertex vertex;
	vertex.position = sf::Vector2f(worldX, worldY);
	vertex.texCoords = sf::Vector2f(texCoordX, texCoordY);
	vertex.color = color;

	m_vertex_array.append(vertex);
}

void ParticleNode::compute_vertices() const {
	sf::Vector2f size(m_texture.getSize());
	sf::Vector2f half = size / 2.f;

	// Refill vertex array
	m_vertex_array.clear();
  for (const Particle& particle : m_particles) {
		sf::Vector2f pos = particle.position;
		sf::Color color = particle.color;

		float ratio = particle.lifetime.asSeconds() / Table[m_type].lifetime.asSeconds();
		color.a = static_cast<sf::Uint8>(255 * std::max(ratio, 0.f));

		add_vertex(pos.x - half.x, pos.y - half.y, 0.f,    0.f,    color);
		add_vertex(pos.x + half.x, pos.y - half.y, size.x, 0.f,    color);
		add_vertex(pos.x + half.x, pos.y + half.y, size.x, size.y, color);
		add_vertex(pos.x - half.x, pos.y + half.y, 0.f,    size.y, color);
	}
}
