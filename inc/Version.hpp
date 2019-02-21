#ifndef __VERSION_HPP
#define __VERSION_HPP
#include <string>

/// \brief Aspen engine namespace
namespace Aspen
{
/// \brief Version namespace
namespace Version
{
/// \brief Release tiers
enum TIER
{
  /// \brief Unknown/default value
  UNKNOWN = 0,
  /// \brief Prealpha build
  PREALPHA,
  /// \brief Prealpha alias
  NIGHTLY = PREALPHA,
  /// \brief Alpha build
  ALPHA,
  /// \brief Beta build
  BETA,
  /// \brief Release build
  RELEASE,
  /// \brief Release alias
  GAMMA = RELEASE,
  /// \brief Total number of enum values
  TOTAL
};

/// \brief Returns the TIER as a string
/// \param tier TIER to represent
/// \return String representation of the given TIER
std::string TierString(const TIER tier);

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
  /// \return _major
  int Major() const;
  /// \brief Returns minor version number
  /// \return _minor
  int Minor() const;
  /// \brief Returns version revision number
  /// \return _revision
  int Revision() const;
  /// \brief Returns version release tier
  /// \return _tier
  TIER Tier() const;

  /// \brief Creates a string of the version
  /// \return String format of the version
  ///         _tier-_major._minor._revision
  std::string ToString() const;

  /// \brief Comparison operator
  /// \param rhs Version on the right of the operator
  /// \return True if both versions are equal
  ///         False otherwise
  bool operator==(Version &rhs) const;
  /// \brief Comparison operator
  /// \param rhs Version on the right of the operator
  /// \return True if both versions are not equal
  ///         False otherwise
  bool operator!=(Version &rhs) const;
  /// \brief Comparison operator
  /// \param rhs Version on the right of the operator
  /// \return True if the left version is less than the right
  ///         False otherwise
  bool operator<(Version &rhs) const;
  /// \brief Comparison operator
  /// \param rhs Version on the right of the operator
  /// \return True if the left version is greater than than the right
  ///         False otherwise
  bool operator>(Version &rhs) const;
  /// \brief Comparison operator
  /// \param rhs Version on the right of the operator
  /// \return True if the left version is less than or equal to the right
  ///         False otherwise
  bool operator<=(Version &rhs) const;
  /// \brief Comparison operator
  /// \param rhs Version on the right of the operator
  /// \return True if the left version is greater than or equal to the right
  ///         False otherwise
  bool operator>=(Version &rhs) const;

  /// \brief Float conversion
  ///        Loses precision of revision and build tier
  /// \return _major._minor
  operator float() const;
  /// \brief Float conversion
  ///        Loses precision of revision and build tier
  /// \return _major._minor
  operator double() const;
  /// \brief String conversion
  /// \return ToString()
  operator std::string() const;
};
} // namespace Version
} // namespace Aspen

#endif
