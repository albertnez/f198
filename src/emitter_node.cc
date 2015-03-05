#include "emitter_node.h"
#include "particle_node.h"
#include "command_queue.h"
#include "command.h"


EmitterNode::EmitterNode(Particle::Type type) 
    : SceneNode(),
      m_accumulated_time(sf::Time::Zero),
      m_type(type),
      m_particle_system(nullptr) {
}

void EmitterNode::update_current(sf::Time dt, CommandQueue& commands) {
	if (m_particle_system) {
		emit_particles(dt);
	}
	else {
		// Find particle node with the same type as emitter node
		auto finder = [this] (ParticleNode& container, sf::Time) {
			if (container.get_particle_type() == m_type)
				m_particle_system = &container;
		};

		Command command;
		command.category = Category::ParticleSystem;
		command.action = derived_action<ParticleNode>(finder);

		commands.push(command);
	}
}

void EmitterNode::emit_particles(sf::Time dt) {
	const float emission_rate = 30.f;
	const sf::Time interval = sf::seconds(1.0f) / emission_rate;

	m_accumulated_time += dt;

	while (m_accumulated_time > interval) {
		m_accumulated_time -= interval;
		m_particle_system->add_particle(get_world_position());
	}
}
