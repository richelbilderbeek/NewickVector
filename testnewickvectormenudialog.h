#ifndef TESTNEWICKVECTORMENUDIALOG_H
#define TESTNEWICKVECTORMENUDIALOG_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include "menudialog.h"
#pragma GCC diagnostic pop

namespace ribi {

struct TestNewickVectorMenuDialog final : public ::ribi::MenuDialog
{
  TestNewickVectorMenuDialog();
  int ExecuteSpecific(const std::vector<std::string>& argv) noexcept override;
  About GetAbout() const noexcept override;
  Help GetHelp() const noexcept override;
  std::string GetVersion() const noexcept override;
  std::vector<std::string> GetVersionHistory() const noexcept override;
};

} //~namespace ribi

#endif // TESTNEWICKVECTORMENUDIALOG_H
