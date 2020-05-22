#include "Component.hpp"

#include "Label.hpp"
#include "Field.hpp"
#include "Parameter.hpp"
#include "ScreenComponent.hpp"

#include <Mpc.hpp>

#include <string>

using namespace std;
using namespace mpc::lcdgui;

Component::Component(const string& name)
{
	this->name = name;
}

bool Component::shouldNotDraw(vector<vector<bool>>* pixels)
{
	if (!IsDirty()) {
		return true;
	}

	if (hidden)
	{
		Clear(pixels);
		dirty = false;
		return true;
	}

	return false;
}

weak_ptr<Parameter> Component::findParameter(const string& name)
{
	for (auto& c : children)
	{
		auto candidate = dynamic_pointer_cast<Parameter>(c);

		if (candidate && candidate->getName().compare(name) == 0)
		{
			return candidate;
		}

		auto secondCandidate = c->findParameter(name).lock();

		if (secondCandidate)
		{
			return secondCandidate;
		}
	}

	return {};
}

weak_ptr<Label> Component::findLabel(const string& name)
{
	for (auto& c : children)
	{
		auto candidate = dynamic_pointer_cast<Label>(c);

		if (candidate && candidate->getName().compare(name) == 0)
		{
			return candidate;
		}

		auto secondCandidate = c->findLabel(name).lock();

		if (secondCandidate)
		{
			return secondCandidate;
		}
	}

	return {};
}

weak_ptr<Field> Component::findField(const string& name)
{
	for (auto& c : children)
	{
		auto candidate = dynamic_pointer_cast<Field>(c);

		if (candidate && candidate->getName().compare(name) == 0)
		{
			return candidate;
		}

		auto secondCandidate = c->findField(name).lock();

		if (secondCandidate)
		{
			return secondCandidate;
		}
	}

	return {};
}

vector<weak_ptr<Label>> Component::findLabels()
{
	vector<weak_ptr<Label>> result;
	for (auto& c : children)
	{
		auto candidate = dynamic_pointer_cast<Label>(c);
		if (candidate)
		{
			result.push_back(candidate);
		}

		for (auto& Label : c->findLabels())
		{
			result.push_back(Label);
		}
	}
	return result;
}

vector<weak_ptr<Field>> Component::findFields()
{
	vector<weak_ptr<Field>> result;
	for (auto& c : children)
	{
		auto candidate = dynamic_pointer_cast<Field>(c);
		if (candidate)
		{
			result.push_back(candidate);
		}

		for (auto& field : c->findFields())
		{
			result.push_back(field);
		}
	}
	return result;
}

vector<weak_ptr<Parameter>> Component::findParameters()
{
	vector<weak_ptr<Parameter>> result;
	for (auto& c : children)
	{
		if (dynamic_pointer_cast<Parameter>(c))
		{
			result.push_back(dynamic_pointer_cast<Parameter>(c));
		}
		
		for (auto& parameter : c->findParameters())
		{
			result.push_back(parameter);
		}
	}
	return result;
}

weak_ptr<Component> Component::addChild(shared_ptr<Component> child)
{
	children.push_back(move(child));
	return children.back();
}

void Component::removeChild(weak_ptr<Component> child)
{
	if (!child.lock())
	{
		return;
	}

	for (auto& c : children)
	{
		if (c == child.lock())
		{
			children.erase(find(begin(children), end(children), child.lock()));
			break;
		}
	}
}

void Component::addChildren(vector<shared_ptr<Component>> children)
{
	for (auto& c : children)
	{
		this->children.push_back(move(c));
	}
}

weak_ptr<Component> Component::findChild(const string& name)
{
	for (auto& c : children)
	{
		if (c->getName().compare(name) == 0)
		{
			return c;
		}

		auto candidate = c->findChild(name).lock();
		if (candidate)
		{
			return candidate;
		}
	}
	return {};
}

void Component::Draw(vector<vector<bool>>* pixels)
{
	if (hidden || !IsDirty())
	{
		return;
	}

	for (auto& c : children)
	{
		c->Draw(pixels);
	}

	dirty = false;
}

const string& Component::getName()
{
	return name;
}

void Component::Hide(bool b) 
{ 
	if (hidden != b)
	{
		hidden = b;
		SetDirty();
	} 
}

void Component::setSize(int w, int h) {
	this->w = w;
	this->h = h;
	SetDirty();
}

void Component::setLocation(int x, int y) {
	this->x = x;
	this->y = y;
	SetDirty();
}

MRECT Component::getDirtyArea() {
	MRECT res;
	for (auto c : children)
	{
		res = res.Union(&c->getDirtyArea());
	}

	if (dirty) {
		auto rect = getRect();
		res = res.Union(&rect);
	}

	return res;
}

void Component::SetDirty() 
{ 
	dirty = true;
}

void Component::SetDirtyRecursive()
{
	
	for (auto& c : children)
	{
		c->SetDirtyRecursive();
	}
	SetDirty();
}

bool Component::IsHidden()
{ 
	return hidden; 
}

bool Component::IsDirty()
{ 
	auto dirtyChild = false;

	for (auto& c : children)
	{
		if (c->IsDirty())
		{
			dirtyChild = true;
			break;
		}

	}

	if (dirtyChild)
	{
		return true;
	}

	return dirty;
}

MRECT Component::getRect() {
	return MRECT(x, y, x + w, y + h);
}

void Component::Clear(vector<vector<bool>>* pixels) {
	auto r = getRect();
	for (int i = r.L; i < r.R - 1; i++) {
		if (i < 0)
		{
			continue;
		}
		for (int j = r.T; j < r.B; j++) {
			(*pixels)[i][j] = false;
		}
	}
}

vector<weak_ptr<Component>> Component::findHiddenChildren()
{
	vector<weak_ptr<Component>> result;
	for (auto& c : children)
	{
		if (c->IsHidden() && c->IsDirty())
		{
			result.push_back(c);
		}

		for (auto& c1 : c->findHiddenChildren())
		{
			result.push_back(c1);
		}
	}

	return result;
}

weak_ptr<ScreenComponent> Component::findScreenComponent()
{
	for (auto& c : children)
	{
		auto candidate = dynamic_pointer_cast<ScreenComponent>(c);
		
		if (candidate)
		{
			return candidate;
		}

		auto childCandidate = c->findScreenComponent().lock();

		if (childCandidate)
		{
			return childCandidate;
		}
	}
	
	return {};
}
