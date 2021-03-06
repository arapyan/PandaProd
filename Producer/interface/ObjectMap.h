#ifndef PandaProd_Producer_ObjectMap_h
#define PandaProd_Producer_ObjectMap_h

#include "DataFormats/Common/interface/Ptr.h"
#include "PandaTree/Framework/interface/Object.h"

#include <map>
#include <string>
#include <utility>
#include <typeinfo>

//! Abstract base to handle ObjectMaps for different types in a single container
class ObjectMapBase {
 public:
  typedef std::pair<size_t, size_t> MapId;

  virtual ~ObjectMapBase() {}
  virtual void clear() {};
  virtual MapId getId() const { return MapId(); };
};

//! Actual EDM <-> panda map
template<class EDM, class PANDA>
class ObjectMap : public ObjectMapBase {
  typedef edm::Ptr<EDM> EDMPtr;

 public:
  std::map<EDMPtr, PANDA*> fwdMap;
  std::map<PANDA*, EDMPtr> bwdMap;

  void clear() override { fwdMap.clear(); bwdMap.clear(); }
  MapId getId() const override { return MapId(typeid(EDM).hash_code(), typeid(PANDA).hash_code()); }

  void add(EDMPtr const& edmRef, PANDA& pandaObj) { fwdMap.emplace(edmRef, &pandaObj); bwdMap.emplace(&pandaObj, edmRef); }
};

//! ObjectMap for a single filler
class FillerObjectMap : public std::map<ObjectMapBase::MapId, ObjectMapBase*> {
 public:
  ~FillerObjectMap() { for (auto& m : *this) delete m.second; }

  void clearMaps() { for (auto& m : *this) m.second->clear(); }

  template<class EDM, class PANDA>
  ObjectMap<EDM, PANDA>& get();

  template<class EDM, class PANDA>
  ObjectMap<EDM, PANDA> const& get() const;
};

typedef std::map<std::string, FillerObjectMap> ObjectMapStore;

template<class EDM, class PANDA>
ObjectMap<EDM, PANDA>&
FillerObjectMap::get()
{
  ObjectMapBase::MapId id(typeid(EDM).hash_code(), typeid(PANDA).hash_code());

  auto sItr(find(id));

  if (sItr == end())
    sItr = emplace(id, new ObjectMap<EDM, PANDA>).first;

  return static_cast<ObjectMap<EDM, PANDA>&>(*sItr->second);
}

template<class EDM, class PANDA>
ObjectMap<EDM, PANDA> const&
FillerObjectMap::get() const
{
  ObjectMapBase::MapId id(typeid(EDM).hash_code(), typeid(PANDA).hash_code());
  return static_cast<ObjectMap<EDM, PANDA> const&>(*at(id));
}

#endif
