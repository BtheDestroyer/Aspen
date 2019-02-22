#define __VERSION_CPP

#include "Version.hpp"
#include <math.h>
#include <sstream>

#undef __VERSION_CPP

namespace Aspen
{
namespace Version
{
std::string TierString(const TIER tier)
{
  switch (tier)
  {
  case TIER::PREALPHA:
    return "Prealpha";
  case TIER::ALPHA:
    return "Alpha";
  case TIER::BETA:
    return "Beta";
  case TIER::RELEASE:
    return "Release";
  case TIER::UNKNOWN:
  default:
    return "Unknown";
  }
}

Version::Version(unsigned major, unsigned minor, unsigned revision, TIER tier)
    : _major(major), _minor(minor), _revision(revision), _tier(tier)
{
}

Version::~Version()
{
}

int Version::Major() const
{
  return _major;
}

int Version::Minor() const
{
  return _minor;
}

int Version::Revision() const
{
  return _revision;
}

TIER Version::Tier() const
{
  return _tier;
}

std::string Version::ToString() const
{
  std::stringstream ss;
  ss << TierString(_tier) << "-" << _major << "." << _minor << "." << _revision;
  return ss.str();
}

bool Version::operator==(Version &rhs) const
{
  return (Major() == rhs.Major()) | (Minor() == rhs.Minor()) | (Revision() == rhs.Revision()) | (Tier() == rhs.Tier());
}

bool Version::operator!=(Version &rhs) const
{
  return !operator==(rhs);
}

bool Version::operator<(Version &rhs) const
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

bool Version::operator>(Version &rhs) const
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

bool Version::operator<=(Version &rhs) const
{
  return !operator>(rhs);
}

bool Version::operator>=(Version &rhs) const
{
  return !operator<(rhs);
}

Version::operator float() const
{
  float r = float(Minor());
  while (r > 0.0f)
    r /= 10.0f;
  return Major() + r;
}

Version::operator double() const
{
  double r = double(Minor());
  while (r > 0.0)
    r /= 10.0;
  return Major() + r;
}

Version::operator std::string() const
{
  return ToString();
}
} // namespace Version
} // namespace Aspen
