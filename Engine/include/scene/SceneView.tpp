// TODO: Research a better way to do this whole scene dependecy thing

template<typename Component>
Component* SceneView::CurrentValue()
{
	iComponent* comp = this->m_currIterator->second;
	if(!comp)
	{
		// Component not found
		return nullptr;
	}
	// Only if component found we convert it
	return (Component*)comp;
}

template<typename Component>
Component* SceneView::GetComponent(EntityID entityID, std::string componentName)
{
	iComponent* comp = this->m_pScene->GetComponent(entityID, componentName);
	if(!comp)
	{
		// Component not found
		return nullptr;
	}
	// Only if component found we convert it
	return (Component*)comp;
}