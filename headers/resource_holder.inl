
template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::load(Identifier id, 
                                                const std::string& filename) {
	// Create and load resource
	std::unique_ptr<Resource> resource(new Resource());
	if (!resource->loadFromFile(filename))
		throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);

	// If loading successful, insert resource to map
	insert_resource(id, std::move(resource));
}

template <typename Resource, typename Identifier>
template <typename Parameter>
void ResourceHolder<Resource, Identifier>::load(Identifier id, 
                                                const std::string& filename,
                                                const Parameter& param) {
	// Create and load resource
	std::unique_ptr<Resource> resource(new Resource());
	if (!resource->loadFromFile(filename, param))
		throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);

	// If loading successful, insert resource to map
	insert_resource(id, std::move(resource));
}

template <typename Resource, typename Identifier>
Resource& ResourceHolder<Resource, Identifier>::get(Identifier id) {
	auto found = m_resources.find(id);
	assert(found != m_resources.end());

	return *found->second;
}

template <typename Resource, typename Identifier>
const Resource& ResourceHolder<Resource, Identifier>::get(Identifier id) const {
	auto found = m_resources.find(id);
	assert(found != m_resources.end());

	return *found->second;
}

template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::insert_resource(
    Identifier id, 
    std::unique_ptr<Resource> resource) {
	// Insert and check success
	auto inserted = m_resources.insert(std::make_pair(id, std::move(resource)));
	assert(inserted.second);
}
