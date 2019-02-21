#ifndef __VERSION_HPP
#define __VERSION_HPP

/// \brief Aspen engine namespace
namespace Aspen
{
/// \brief Version namespace
namespace Version
{
enum TIER
{
  UNKNOWN = 0,
  PREALPHA,
  ALPHA,
  BETA,
  RELEASE,
  TOTAL,
  GAMMA = RELEASE,
  NIGHTLY = PREALPHA
};

/// \brief Version class
///        Contains version information and comparison operators
class Version
{
  /// \brief Major version number
  const unsigned _major;
  /// \brief Minor version number
  const unsigned _minor;
  /// \brief Version revision
  const unsigned _revision;
  /// \brief Version release tier
  const TIER _tier;

public:
  /// \brief Constructor
  /// \param major Major version number
  /// \param minor Minor version number
  /// \param revision Version revision
  /// \param tier Version release tier
  Version(unsigned major, unsigned minor = 0, unsigned revision = 0, TIER tier = UNKNOWN);
  /// \brief Destructor
  ~Version();

  /// \brief Returns major version number
  int Major();
  /// \brief Returns minor version number
  int Minor();
  /// \brief Returns version revision number
  int Revision();
  /// \brief Returns version release tier
  TIER Tier();

  bool operator==(Version &rhs);
  bool operator!=(Version &rhs);
  bool operator<(Version &rhs);
  bool operator>(Version &rhs);
  bool operator<=(Version &rhs);
  bool operator>=(Version &rhs);
  operator float();
  operator double();
};
} // namespace Version
} // namespace Aspen

#endif
