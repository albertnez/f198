#ifndef PARTICLE_NODE_H
#define PARTICLE_NODE_H

#include "scene_node.h"
#include "particle.h"
#include "resource_identifiers.h"

#include <SFML/Graphics/VertexArray.hpp>

#include <deque>


class ParticleNode : public SceneNode {
public:
  explicit ParticleNode(Particle::Type m_type, const TextureHolder& textures);

  void add_particle(sf::Vector2f position);
  void add_particle(sf::Vector2f position, sf::Vector2f dir);
  Particle::Type get_particle_type() const;
  unsigned int get_category() const override;

private:
  void update_current(sf::Time dt, CommandQueue& commands) override;
  void draw_current(sf::RenderTarget& target, 
                    sf::RenderStates states) const override;
  
  void add_vertex(float world_x, float world_y, float tex_coord_x, 
                  float tex_coord_y, const sf::Color& color) const;

  void compute_vertices() const;

  std::deque<Particle> m_particles;
  const sf::Texture& m_texture;
  Particle::Type m_type;

  mutable sf::VertexArray	m_vertex_array;
  mutable bool m_needs_vertex_update;
};

#endif // PARTICLE_NODE_H
