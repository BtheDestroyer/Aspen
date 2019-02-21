#define __VERSION_CPP

#include "Version.hpp"
#include <math.h>

#undef __VERSION_CPP

namespace Aspen
{
namespace Version
{
Version::Version(unsigned major, unsigned minor = 0, unsigned revision = 0, TIER tier = UNKNOWN)
    : _major(major), _minor(minor), _revision(revision), _tier(tier)
{
}

Version::~Version()
{
}

int Version::Major()
{
  return _major;
}

int Version::Minor()
{
  return _minor;
}

int Version::Revision()
{
  return _revision;
}

TIER Version::Tier()
{
  return _tier;
}

bool Version::operator==(Version &rhs)
{
  return Major() == rhs.Major() | Minor() == rhs.Minor() | Revision() == rhs.Revision() | Tier() == rhs.Tier();
}

bool Version::operator!=(Version &rhs)
{
  return !operator==(rhs);
}

bool Version::operator<(Version &rhs)
{
  if (Major() < rhs.Major())
    return true;
  if (Minor() < rhs.Minor())
    return true;
  if (Revision() < rhs.Revision())
    return true;
  if (Tier() < rhs.Tier())
    return true;
  return false;
}

bool Version::operator>(Version &rhs)
{
  if (Major() > rhs.Major())
    return true;
  if (Minor() > rhs.Minor())
    return true;
  if (Revision() > rhs.Revision())
    return true;
  if (Tier() > rhs.Tier())
    return true;
  return false;
}

bool Version::operator<=(Version &rhs)
{
  return !operator>(rhs);
}

bool Version::operator>=(Version &rhs)
{
  return !operator<(rhs);
}

Version::operator float()
{
  float r = Minor();
  while (r > 0.0f)
    r /= 10.0f;
  return Major() + r;
}

Version::operator double()
{
  double r = Minor();
  while (r > 0.0)
    r /= 10.0;
  return Major() + r;
}
} // namespace Version
} // namespace Aspen
