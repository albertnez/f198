#ifndef EMITTER_NODE_H
#define EMITTER_NODE_H

#include "scene_node.h"
#include "particle.h"


class ParticleNode;

class EmitterNode : public SceneNode {
public:
  explicit				EmitterNode(Particle::Type type);

private:
  virtual void update_current(sf::Time dt, CommandQueue& commands);
  void emit_particles(sf::Time dt);

  sf::Time m_accumulated_time;
  Particle::Type m_type;
  ParticleNode* m_particle_system;
};

#endif // EMITTER_NODE_H
