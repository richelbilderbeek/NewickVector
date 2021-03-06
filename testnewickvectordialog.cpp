//---------------------------------------------------------------------------
/*
TestNewickVector, GUI tool to test NewickVector
Copyright (C) 2011-2015 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/ToolTestNewickVector.htm
//---------------------------------------------------------------------------
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#include "testnewickvectordialog.h"

#include <cassert>
#include <iostream>

#include <boost/numeric/conversion/cast.hpp>
#include <boost/lexical_cast.hpp>

#include "fileio.h"
#include "binarynewickvector.h"
#include "fuzzy_equal_to.h"
#include "newick.h"
#include "newickvector.h"
#include "twodigitnewick.h"

#pragma GCC diagnostic pop

ribi::TestNewickVectorDialog::TestNewickVectorDialog() noexcept
  : m_analyse_calculation{false},
    m_compare{false},
    m_max_complexity_str{},
    m_newick_str{},
    m_theta_str{},
    m_newick{},
    m_theta{},
    m_complexity{},
    m_denominator{},
    m_n_combinations{},
    m_n_symmetries{},
    m_text{}
{

}

void ribi::AddCoefficientsAndProbabilities(
  std::vector<std::string>& v,
  const std::vector<double>& coefficients,
  const std::vector<double>& probabilities
) noexcept
{
  assert(coefficients.size() == probabilities.size());
  const int sz{static_cast<int>(coefficients.size())};
  for (int i=0; i!=sz; ++i)
  {
    std::string s = (i==0
      ? "       =  "
      : "         +")
    ;
    s += " ( "
      + boost::lexical_cast<std::string>(coefficients[i])
      + " * "
      + boost::lexical_cast<std::string>(probabilities[i])
      + " )"
    ;
    v.push_back(s);
  }
}

void ribi::TestNewickVectorDialog::AddCoefficientsCs(
  std::vector<std::string>& v,
  const std::vector<NewickFrequencyPair>& simpler,
  const std::vector<double>& coefficients
) const noexcept
{
  const int n_simpler = boost::numeric_cast<int>(simpler.size());
  for (int i=0; i!=n_simpler; ++i)
  {
    const int f = simpler[i].second;
    v.push_back(
      "c'"
      + boost::lexical_cast<std::string>(i+1)
      + " = "
      + std::string(f == 1 ? "t / d" : "(f*(f-1)) / d")
      + " = "
      + boost::lexical_cast<std::string>(f == 1
        ? m_theta
        : boost::lexical_cast<double>(f*(f-1)))
      + " / "
      + boost::lexical_cast<std::string>(m_denominator)
      + " = "
      + boost::lexical_cast<std::string>(coefficients[i])
      + " (as f equals "
      + boost::lexical_cast<std::string>(f)
      + ")"
    );
  }
}

void ribi::TestNewickVectorDialog::AddCoefficients(
  std::vector<std::string>& v,
  const std::vector<NewickFrequencyPair>& simpler,
  const std::vector<double>& probabilities,
  const std::vector<double>& coefficients
) const noexcept
{
  assert(simpler.size() == probabilities.size());
  assert(probabilities.size() == coefficients.size());

  AddCoefficientsCs(v, simpler, coefficients);
  v.push_back("p(N,t) = SUM(c_i * p_i)");

  AddCoefficientsAndProbabilities(v, coefficients, probabilities);

  //Hand-calculate probability
  const double p_by_hand = CalculateProbabilityByHand(coefficients, probabilities);
  v.push_back(
    "       = "
    + boost::lexical_cast<std::string>(p_by_hand)
    + " (hand-calculated)"
  );
  const double p_at_once = CalculateProbabilityNewickVector(m_newick->ToStr(),m_theta);
  v.push_back(
    std::string(ribi::fuzzy_equal_to()(p_by_hand,p_at_once)
      ? "       = "
      : "       != ")
    + boost::lexical_cast<std::string>(p_at_once)
    + " (calculated at once by NewickVector)"
  );
  if (m_compare
    && (  newick::IsUnaryNewick(m_newick->Peek())
       || newick::IsBinaryNewick(m_newick->Peek()) ) )
  {
    const double p_two_digit_newick{
      CalculateProbabilityTwoDigitNewick(m_newick->ToStr(),m_theta)
    };
    v.push_back(
      std::string(ribi::fuzzy_equal_to()(p_two_digit_newick,p_at_once)
        ? "       = "
        : "       != ")
      + boost::lexical_cast<std::string>(p_two_digit_newick)
      + " (calculated at once by TwoDigitNewick)"
    );
  }
  v.push_back(std::string(80,'-'));
}

void ribi::TestNewickVectorDialog::AddDerivedNewicks(
  std::vector<std::string>& v,
  const std::vector<NewickFrequencyPair>& simpler,
  const std::vector<double>& probabilities
) const noexcept
{
  const int n_simpler = boost::numeric_cast<int>(simpler.size());
  for (int i=0; i!=n_simpler; ++i)
  {
    v.push_back(
      "N'"
      + boost::lexical_cast<std::string>(i+1)
      + " = "
      + newick::NewickToString(simpler[i].first));
  }
  v.push_back(" ");
  v.push_back("For t = "
    + boost::lexical_cast<std::string>(m_theta)
    + ":");
  v.push_back(" ");
  //Display probabilities
  for (int i=0; i!=n_simpler; ++i)
  {
    v.push_back(
      "p'"
      + boost::lexical_cast<std::string>(i+1)
      + " = "
      + boost::lexical_cast<std::string>(probabilities[i])
      + " (calculated at once with NewickVector)"
    );
  }
}


void ribi::TestNewickVectorDialog::AddGeneralInfo(
  std::vector<std::string>& v
) const noexcept
{
  v.push_back(std::string(80,'-'));
  v.push_back("Calculation");
  v.push_back(std::string(80,'-'));
  v.push_back("N = the phylogeny = " + m_newick_str);
  v.push_back("t = theta = " + m_theta_str);
  v.push_back("p(N,t) = probability = SUM(c_i * p_i)");
  v.push_back("c(N,t) = coefficient");
  v.push_back("D(N,t) = denominator = "
    + boost::lexical_cast<std::string>(m_denominator)
  );
}

void ribi::TestNewickVectorDialog::Analyse() noexcept
{
  //Store the data
  m_newick.reset(new NewickVector(m_newick_str));
  m_theta = boost::lexical_cast<double>(m_theta_str);
  m_complexity = bigIntegerToString(m_newick->CalcComplexity());
  m_denominator = m_newick->CalcDenominator(m_theta);

  m_text.push_back("Newick complexity: "
      + m_complexity);

  AnalyseArity();
  AnalyseSimplerNewicks();
  AnalyseCalculation();

  AnalyseRootBranches();

  if (newick::IsBinaryNewick(m_newick->Peek()))
  {
    m_n_combinations = bigIntegerToString(m_newick->CalcNumOfCombinations());
    m_n_symmetries = bigIntegerToString(m_newick->CalcNumOfSymmetries());
  }
  else
  {
    m_n_combinations = "N/A (not a binary Newick)";
    m_n_symmetries = "N/A (not a binary Newick)";
  }
  m_text.push_back("Number of combinations: " + m_n_combinations);
  m_text.push_back("Number of symmetries: " + m_n_symmetries);
  m_text.push_back("Denominator: " + boost::lexical_cast<std::string>(m_denominator));
}

void ribi::TestNewickVectorDialog::AnalyseArity() noexcept
{
  //Check if simple Newick
  if (newick::IsSimple(m_newick->Peek()))
  {
    m_text.push_back("Simple Newick: Yes");
    m_text.push_back("Ewens probability: "
      + boost::lexical_cast<std::string>(
        m_newick->CalcProbabilitySimpleNewick(m_theta)));
  }
  else
  {
    m_text.push_back("Simple Newick: No");
  }
  //Check if binary Newick
  if (newick::IsBinaryNewick(m_newick->Peek()))
  {
    m_text.push_back("Binary Newick: Yes");
  }
  else
  {
    m_text.push_back("Binary Newick: No");
  }
  if (newick::IsTrinaryNewick(m_newick->Peek()))
  {
    m_text.push_back("Trinary Newick: Yes");
  }
  else
  {
    m_text.push_back("Trinary Newick: No");
  }
}

void ribi::TestNewickVectorDialog::AnalyseCalculation() noexcept
{
  if (!m_analyse_calculation) return;

  typedef std::pair<std::vector<int>,int> NewickFrequencyPair;
  const std::vector<NewickFrequencyPair> simpler
    = newick::GetSimplerNewicksFrequencyPairs(m_newick->Peek());
  //Collect coeficients
  std::vector<double> coefficients;
  for(const NewickFrequencyPair& p: simpler)
  {
    const int f = p.second;
    coefficients.push_back(f == 1
      ? m_theta / m_denominator
      : boost::numeric_cast<double>(f*(f-1)) / m_denominator);
  }
  assert(coefficients.size() == simpler.size());
  //Collect probabilities
  std::vector<double> probabilities;
  for(const NewickFrequencyPair& p: simpler)
  {
    const NewickVector n(p.first);
    probabilities.push_back(CalculateProbabilityNewickVector(n.ToStr(),m_theta));
  }
  assert(probabilities.size() == simpler.size());

  //Display general info
  AddGeneralInfo(m_text);

  //Display derived Newicks
  AddDerivedNewicks(m_text, simpler, probabilities);

  //Display coefficients
  AddCoefficients(m_text, simpler, probabilities, coefficients);
}

void ribi::TestNewickVectorDialog::AnalyseRootBranches() noexcept
{
  if(!newick::IsUnaryNewick(m_newick->Peek()))
  {
    std::string text = "(X,Y) =\n";
    const std::vector<std::vector<int> > b
      = newick::GetRootBranches(m_newick->Peek());
    try
    {
      for(const std::vector<int>& c: b)
      {
        newick::CheckNewick(c);
        text+="  " + newick::NewickToString(c);
      }
    }
    catch (std::exception& e)
    {
      text+=std::string(e.what());
    }
    text+="\n";
    m_text.push_back(text);
  }
}

void ribi::TestNewickVectorDialog::AnalyseSimplerNewicks() noexcept
{
  typedef std::pair<std::vector<int>,int> NewickFrequencyPair;
  const std::vector<NewickFrequencyPair> simpler
    = newick::GetSimplerNewicksFrequencyPairs(m_newick->Peek());
  std::string text = "Simpler Newicks:\n";
  for(const NewickFrequencyPair& simple_pair: simpler)
  {
    const std::vector<int>& simple = simple_pair.first;
    const int f = simple_pair.second;
    try
    {
      newick::CheckNewick(simple);
      text+="  ";
      text+=newick::NewickToString(simple);
      text+=" (from f = ";
      text+=boost::lexical_cast<std::string>(f);
      text+=")\n";
    }
    catch (std::exception& e)
    {

      //Should not get here
      text += std::string(e.what()) + "\n";
    }
  }
  if (simpler.empty()) text+=" [none]\n";
  //Pop the trailing '\n'
  text.resize(text.size() - 1);
  m_text.push_back(text.c_str());
}

void ribi::TestNewickVectorDialog::AutoCalculate() noexcept
{
  m_text.resize(0);

  if (!CheckNewick()) return;
  if (!CheckTheta()) return;
  if (!CheckMaxComplexity()) return;

  const BigInteger max_complexity
    = stringToBigInteger(m_max_complexity_str);
  if (newick::CalcComplexity(newick::StringToNewick(m_newick_str))
    > max_complexity)
  {
    m_text.push_back(
      "Newick too complex to auto-calculate. "
      "Press 'Calculate' or increase the value for auto-calculate"
    );
    return;
  }
  Analyse();
}

void ribi::TestNewickVectorDialog::Calculate() noexcept
{
  m_text.resize(0);
  if (!CheckNewick()) return;
  if (!CheckTheta()) return;
  Analyse();

}

double ribi::CalculateProbabilityByHand(
  const std::vector<double>& probabilities,
  const std::vector<double>& coefficients
) noexcept
{
  assert(probabilities.size() == coefficients.size());
  double p_by_hand = 0.0;
  const int sz{static_cast<int>(probabilities.size())};
  for (int i=0; i!=sz; ++i)
  {
    p_by_hand += (coefficients[i] * probabilities[i]);
  }
  return p_by_hand;
}


bool ribi::TestNewickVectorDialog::CheckMaxComplexity() noexcept
{
  try
  {
    boost::lexical_cast<int>(m_max_complexity_str);
  }
  catch (boost::bad_lexical_cast&)
  {
    m_text.push_back("Valid maximum autocalc complexity: No");
    return false;
  }
  m_text.push_back("Valid maximum autocalc complexity: Yes");
  return true;
}

bool ribi::TestNewickVectorDialog::CheckNewick() noexcept
{
  if (!newick::IsNewick(m_newick_str))
  {
    m_text.push_back("Valid Newick: No");
    //No Newick, why not?
    try
    {
      newick::CheckNewick(m_newick_str);
    }
    catch (std::exception& e)
    {
      //Must get here
      m_text.push_back(std::string("Exception: ") + e.what());
    }
    return false;
  }
  m_text.push_back("Valid Newick: Yes");
  return true;
}

bool ribi::TestNewickVectorDialog::CheckTheta() noexcept
{
  try
  {
    boost::lexical_cast<double>(m_theta_str);
  }
  catch (boost::bad_lexical_cast&)
  {
    m_text.push_back("Valid theta: No");
    return false;
  }
  m_text.push_back("Valid theta: Yes");
  return true;
}


void ribi::TestNewickVectorDialog::SetAnalyseCalculation(const bool b) noexcept
{
  m_analyse_calculation = b;
}

void ribi::TestNewickVectorDialog::SetCompareToTwoDigitNewick(const bool b) noexcept
{
  m_compare = b;
}

void ribi::TestNewickVectorDialog::SetMaxComplexity(const std::string& s)
{
  m_max_complexity_str = s;
}

void ribi::TestNewickVectorDialog::SetNewick(const std::string& s)
{
  m_newick_str = s;
}

void ribi::TestNewickVectorDialog::SetTheta(const std::string& s)
{
  m_theta_str = s;
}

