/*WIKI*
== Notice ==
1. This is not an algorithm. However this fit function is a used through the [[Fit]] algorithm.

2. It is renamed from ThermalNeutronBk2BkExpConvPV.

3. ThermalNeutronBk2BkExpConvPVoigt is not a regular peak function to fit individual peaks.  It is not allowed to set FWHM or peak centre to this peak function.

== Summary ==
A thermal neutron back-to-back exponential convoluted with pseuduo-voigt peakshape function is indeed a back-to-back exponential convoluted with pseuduo-voigt peakshape function,
while the parameters :<math>\alpha</math>, :<math>\beta</math> and :<math>\sigma</math> are not directly given, but calculated from a set of parameters that are universal to all peaks in powder diffraction data.

The purpose to implement this peak shape is to perform Le Bail Fit and other data analysis on time-of-flight powder diffractometers' data in Mantid.    It is the peak shape No. 10 in Fullprof.  See Refs. 1.

== Description ==
Thermal neutron back to back exponential convoluted with psuedo voigt peak function is a back to back exponential convoluted with psuedo voigt peak function.
Its difference to a regular back to back exponential convoluted with psuedo voigt peak functiont is that it is a function for all peaks in a TOF powder diffraction pattern,
but not a single peak.

Furthermore, the purpose to implement this function in Mantid is to refine multiple parameters including crystal sample's unit cell parameters.
Therefore, unit cell lattice parameters are also included in this function.

==== Methods are not supported ====
1. setFWHM()
2. setCentre() : peak centre is determined by a set of parameters including lattice parameter, Dtt1, Dtt1t, Zero, Zerot, Dtt2t, Width and Tcross.  Therefore,
it is not allowed to set peak centre to this peak function.

==== Back-to-back exponential convoluted with pseuduo-voigt peakshape function ====

A back-to-back exponential convoluted with pseuduo-voigt peakshape function for is defined as

:<math>  \Omega(X_0) = \int_{\infty}^{\infty}pV(X_0-t)E(t)dt </math>

For back-to-back exponential:
:<math>   E(d, t) = 2Ne^{\alpha(d) t}   (t \leq 0)   </math>
:<math>   E(d, t) = 2Ne^{-\beta(d) t}   (t \geq 0)   </math>
:<math>   N(d)    = \frac{\alpha(d)\beta(d)}{2(\alpha(d)+\beta(d))} </math>

For psuedo-voigt
:<math> pV(x) = \eta L'(x) + (1-\eta)G'(x) </math>

The parameters <math>/alpha</math> and <math>/beta</math> represent the absolute value of the exponential rise and decay constants (modelling the neutron pulse coming from the moderator)
, L'(x) stands for Lorentzian part and G'(x) stands for Gaussian part.  The parameter <math>X_0</math> is the location of the peak; more specifically it represent
the point where the exponentially modelled neutron pulse goes from being exponentially rising to exponentially decaying.

References

1. Fullprof manual

The figure below illustrate this peakshape function fitted to a TOF peak:

[[Image:BackToBackExponentialWithConstBackground.png]]

==== Formula for converting unit from d-spacing to TOF ====
Parameters of back-to-back exponential convoluted psuedo-voigt function are calculated from a set of parameters universal to all peaks in a diffraction pattern.
Therefore, they are functions of peak position, <math>d</math>.

  <math> n_{cross} = \frac{1}{2} erfc(Width(xcross\cdot d^{-1})) </math>

  <math> TOF_e = Zero + Dtt1\cdot d </math>

  <math> TOF_t = Zerot + Dtt1t\cdot d - Dtt2t \cdot d^{-1} </math>

Final Time-of-flight is calculated as:
  <math> TOF = n_{cross} TOF_e + (1-n_{cross}) TOF_t </math>

==== Formular for calculating <math>A(d)</math>, <math>B(d)</math>, <math>\sigma(d)</math> and <math>\gamma(d)</math> ====

* <math>\alpha(d)</math>
  <math>  \alpha^e(d) = \alpha_0^e + \alpha_1^e d_h         </math>
  <math>  \alpha^t(d) = \alpha_0^t - \frac{\alpha_1^t}{d_h} </math>
  <math>  \alpha(d)   = \frac{1}{n\alpha^e + (1-n)\alpha^t} </math>

* <math>\beta(d)</math>
   <math>  \beta^e(d) = \beta_0^e + \beta_1^e d_h </math>
   <math>  \beta^t(d) = \beta_0^t - \frac{\beta_1^t}{d_h} </math>
   <math>  \beta(d)   = \frac{1}{n\alpha^e + (1-n)\beta^t} </math>

* For <math>\sigma_G</math> and <math>\gamma_L</math>, which represent the standard deviation for pseudo-voigt
    <math> \sigma_G^2(d_h) = \sigma_0^2 + (\sigma_1^2 + DST2(1-\zeta)^2)d_h^2 + (\sigma_2^2 + Gsize)d_h^4 </math>

    <math> \gamma_L(d_h) = \gamma_0 + (\gamma_1 + \zeta\sqrt{8\ln2DST2})d_h + (\gamma_2+F(SZ))d_h^2 </math>
    \end{eqnarray}

* The analysis formula for the convoluted peak at <math>d_h</math>
    <math> \Omega(TOF(d_h)) =
        (1-\eta(d_h))N\{e^uerfc(y)+e^verfc(z)\} - \frac{2N\eta}{\pi}\{\Im[e^pE_1(p)]+\Im[e^qE_1(q)]\} </math>
where
    <math> erfc(x) = 1-erf(x) = 1-\frac{2}{\sqrt{\pi}}\int_0^xe^{-u^2}du </math>

    <math> E_1(z) = \int_z^{\infty}\frac{e^{-t}}{t}dt </math>

    <math> u = \frac{1}{2}\alpha(d_h)(\alpha(d_h)\sigma^2(d_h)+2x)  </math>

    <math> y = \frac{\alpha(d_h)\sigma^2(d_h)+x}{\sqrt{2\sigma^2(d_h)}} </math>

    <math> p = \alpha(d_h)x + \frac{i\alpha(d_h)H(d_h)}{2}  </math>

    <math> v = \frac{1}{2}\beta(d_h)(\beta(d_h)\sigma^2(d_h)-2x)    </math>

    <math> z = \frac{\beta(d_h)\sigma^2(d_h)-x}{\sqrt{2\sigma^2(d_h)}}  </math>

    <math> q = -\beta(d_h)x + \frac{i\beta(d_h)H(d_h)}{2}  </math>

<math>erfc(x)</math> and <math>E_1(z)</math> will be calculated numerically.
 *WIKI*/

#include "MantidCurveFitting/ThermalNeutronBk2BkExpConvPVoigt.h"
// #include "MantidAPI/Algorithm.h"
#include "MantidAPI/FunctionFactory.h"
#include "MantidAPI/ParamFunction.h"
#include "MantidKernel/EmptyValues.h"
#include "MantidKernel/MultiThreaded.h"

#include "MantidKernel/ConfigService.h"

#include <gsl/gsl_sf_erf.h>
#include <boost/lexical_cast.hpp>

const double PI = 3.14159265358979323846264338327950288419716939937510582;
const double PEAKRANGE = 5.0;
const double TWO_OVER_PI = 2./PI;
const double NEG_DBL_MAX = -1.*DBL_MAX;

using namespace std;
using namespace Mantid;
using namespace Mantid::API;


namespace Mantid
{
namespace CurveFitting
{
  DECLARE_FUNCTION(ThermalNeutronBk2BkExpConvPVoigt)

  // Get a reference to the logger
  Mantid::Kernel::Logger& ThermalNeutronBk2BkExpConvPVoigt::g_log =
      Kernel::Logger::get("ThermalNeutronBk2BkExpConvPV");

  //----------------------------------------------------------------------------------------------
  /** Constructor
 */
  ThermalNeutronBk2BkExpConvPVoigt::ThermalNeutronBk2BkExpConvPVoigt() : IPowderDiffPeakFunction()
  {
    mHKLSet = false;
  }
    
  //------------------------------------------------------------------------------------------------
  /** Destructor
  */
  ThermalNeutronBk2BkExpConvPVoigt::~ThermalNeutronBk2BkExpConvPVoigt()
  {
  }
  
  //----------------------------------------------------------------------------------------------
  /** Define the fittable parameters
   * Notice that Sig0, Sig1 and Sig2 are NOT the squared value recorded in Fullprof
   */
  void ThermalNeutronBk2BkExpConvPVoigt::init()
  {
    // Peak height (0)
    declareParameter("Height", 1.0);

    // Instrument geometry related (1 ~ 8)
    declareParameter("Dtt1", 1.0, "coefficient 1 for d-spacing calculation for epithermal neutron part");
    declareParameter("Dtt2", 1.0, "coefficient 2 for d-spacing calculation for epithermal neutron part");
    declareParameter("Dtt1t", 1.0, "coefficient 1 for d-spacing calculation for thermal neutron part");
    declareParameter("Dtt2t", 1.0, "coefficient 2 for d-spacing calculation for thermal neutron part");
    declareParameter("Zero", 0.0, "Zero shift for epithermal neutron");
    declareParameter("Zerot", 0.0, "Zero shift for thermal neutron");
    declareParameter("Width", 1.0, "width of the crossover region");
    declareParameter("Tcross", 1.0, "position of the centre of the crossover region");

    // Peak profile related (9 ~ 16) Back to back Expoential
    declareParameter("Alph0",1.6, "exponential constant for rising part of epithermal neutron pulse");
    declareParameter("Alph1",1.5, "exponential constant for rising part of expithermal neutron pulse");
    declareParameter("Beta0",1.6, "exponential constant of decaying part of epithermal neutron pulse");
    declareParameter("Beta1",1.5, "exponential constant of decaying part of epithermal neutron pulse");
    declareParameter("Alph0t",1.6, "exponential constant for rising part of thermal neutron pulse");
    declareParameter("Alph1t",1.5, "exponential constant for rising part of thermal neutron pulse");
    declareParameter("Beta0t",1.6, "exponential constant of decaying part of thermal neutron pulse");
    declareParameter("Beta1t",1.5, "exponential constant of decaying part of thermal neutron pulse");

    // Pseudo-Voigt (17 ~ 22)
    declareParameter("Sig0", 1.0, "variance parameter 1 of the Gaussian component of the psuedovoigt function");
    declareParameter("Sig1", 1.0, "variance parameter 2 of the Gaussian component of the psuedovoigt function");
    declareParameter("Sig2", 1.0, "variance parameter 3 of the Gaussian component of the psuedovoigt function");

    declareParameter("Gam0", 0.0, "FWHM parameter 1 of the Lorentzian component of the psuedovoigt function");
    declareParameter("Gam1", 0.0, "FWHM parameter 2 of the Lorentzian component of the psuedovoigt function");
    declareParameter("Gam2", 0.0, "FWHM parameter 3 of the Lorentzian component of the psuedovoigt function");

    // Lattice parameter (23)
    declareParameter("LatticeConstant", 10.0, "lattice constant for the sample");

    // Unit cell
    m_unitCellSize = 10.0;

    // Set flag
    m_cellParamValueChanged = true;

    return;
  }

  //------------- Public Functions (Overwrite) Set/Get ---------------------------------------------



  //------------- Public Functions (New) Set/Get -------------------------------------------------

  //----------------------------------------------------------------------------------------------
  /** Set Miller Indices for this peak
   */
  void ThermalNeutronBk2BkExpConvPVoigt::setMillerIndex(int h, int k, int l)
  {
    // Check validity and set flag
    if (mHKLSet)
    {
      // Throw exception if tried to reset the miller index
      stringstream errss;
      errss << "ThermalNeutronBk2BkExpConvPVoigt Peak cannot have (HKL) reset.";
      g_log.error(errss.str());
      throw runtime_error(errss.str());
    }
    else
    {
      // Set flag
      mHKLSet = true;
    }

    // Set value
    mH = static_cast<int>(h);
    mK = static_cast<int>(k);
    mL = static_cast<int>(l);

    // Check value valid or not
    if (mH*mH + mK*mK + mL*mL < 1.0E-8)
    {
      stringstream errmsg;
      errmsg << "H = K = L = 0 is not allowed";
      g_log.error(errmsg.str());
      throw std::invalid_argument(errmsg.str());
    }

    return;
  }

  //----------------------------------------------------------------------------------------------
  /** Get Miller Index from this peak
   */
  void ThermalNeutronBk2BkExpConvPVoigt::getMillerIndex(int& h, int &k, int &l)
  {
    h = static_cast<int>(mH);
    k = static_cast<int>(mK);
    l = static_cast<int>(mL);

    return;
  }

    //----------------------------------------------------------------------------------------------
  /** Get peak parameters stored locally
   * Get some internal parameters values including
   * (a) Alpha, (b) Beta, (c) Gamma, (d) Sigma2
   * Exception: if the peak profile parameter is not in this peak, then
   *            return an Empty_DBL
   */
  double ThermalNeutronBk2BkExpConvPVoigt::getPeakParameter(std::string paramname)
  {
    // 1. Calculate peak parameters if required
    if (m_hasNewParameterValue)
    {
      calculateParameters(false);
    }

    // 2. Get value
    double paramvalue;

    if (paramname.compare("Alpha") == 0)
      paramvalue = m_Alpha;
    else if (paramname.compare("Beta") == 0)
      paramvalue = m_Beta;
    else if (paramname.compare("Sigma2") == 0)
      paramvalue = m_Sigma2;
    else if (paramname.compare("Gamma") == 0)
      paramvalue = m_Gamma;
    else if (paramname.compare("d_h") == 0)
      paramvalue = m_dcentre;
    else if (paramname.compare("Eta") == 0)
      paramvalue = m_eta;
    else if (paramname.compare("TOF_h") == 0)
      paramvalue = m_centre;
    else
    {
      stringstream errss;
      errss << "Parameter " << paramname << " does not exist in peak function "
            << this->name() << "'s calculated parameters. "
            << "Candidates are Alpha, Beta, Sigma2, Gamma d_h and Eta. ";
      g_log.error(errss.str());
      throw runtime_error(errss.str());
    }

    return paramvalue;
  }

  //------------- Public Functions (Overwrite) Calculation ---------------------------------------------
  /** Calculate peak parameters (fundamential Back-to-back PV),including
  * alpha, beta, sigma^2, eta, H
  */
  void ThermalNeutronBk2BkExpConvPVoigt::calculateParameters(bool explicitoutput) const
  {
    // Obtain parameters (class) with pre-set order
    double dtt1   = getParameter(1);
    double dtt1t  = getParameter(3);
    double dtt2t  = getParameter(4);
    double zero   = getParameter(5);
    double zerot  = getParameter(6);
    double wcross = getParameter(7);
    double Tcross = getParameter(8);

    double alph0 = getParameter(9);
    double alph1 = getParameter(10);
    double beta0 = getParameter(11);
    double beta1 = getParameter(12);
    double alph0t = getParameter(13);
    double alph1t = getParameter(14);
    double beta0t = getParameter(15);
    double beta1t = getParameter(16);

    double sig0 = getParameter(17);
    double sig1 = getParameter(18);
    double sig2 = getParameter(19);
    double gam0 = getParameter(20);
    double gam1 = getParameter(21);
    double gam2 = getParameter(22);

    double latticeconstant = getParameter(23);

    double dh, tof_h, eta, alpha, beta, H, sigma2, gamma, N;

    // Calcualte Peak Position d-spacing and TOF
    if (m_cellParamValueChanged)
    {
      m_unitCell.set(latticeconstant, latticeconstant, latticeconstant, 90.0, 90.0, 90.0);
      dh = m_unitCell.d(mH, mK, mL);
      m_dcentre = dh;
      m_cellParamValueChanged = false;
    }
    else
    {
      dh = m_dcentre;
    }

    // Calculate all the parameters
    // - Start to calculate alpha, beta, sigma2, gamma,
    double n = 0.5*gsl_sf_erfc(wcross*(Tcross-1/dh));

    double alpha_e = alph0 + alph1*dh;
    double alpha_t = alph0t - alph1t/dh;
    alpha = 1/(n*alpha_e + (1-n)*alpha_t);

    double beta_e = beta0 + beta1*dh;
    double beta_t = beta0t - beta1t/dh;
    beta = 1/(n*beta_e + (1-n)*beta_t);

    double Th_e = zero + dtt1*dh;
    double Th_t = zerot + dtt1t*dh - dtt2t/dh;
    tof_h = n*Th_e + (1-n)*Th_t;

    sigma2 = sig0*sig0 + sig1*sig1*std::pow(dh, 2) + sig2*sig2*std::pow(dh, 4);
    gamma = gam0 + gam1*dh + gam2*std::pow(dh, 2);

    // - Calcualte H for the peak
    calHandEta(sigma2, gamma, H, eta);

    N = alpha*beta*0.5/(alpha+beta);

    // Record recent value
    m_Alpha = alpha;
    m_Beta = beta;
    m_Sigma2 = sigma2;
    m_Gamma = gamma;
    m_fwhm = H;
    m_centre = tof_h;
    m_N = N;
    m_eta = eta;

    // Check whether all the parameters are physical
    if (alpha != alpha || beta != beta || sigma2 != sigma2 || gamma != gamma || H != H || H <= 0.)
    {
      m_parameterValid = false;
    }
    else
    {
      m_parameterValid = true;
    }

    // 5.Debug output
    if (explicitoutput)
    {
      stringstream errss;
      errss << "alpha = " << alpha << ", beta = " << beta
            << ", N = " << N << "\n";
      errss << "  n = " << n << ", alpha_e = " << alpha_e << ", alpha_t = " << alpha_t << "\n";
      errss << " dh = " << dh << ", alph0t = " << alph0t << ", alph1t = " << alph1t
            << ", alph0 = " << alph0 << ", alph1 = " << alph1 << "\n";
      errss << "  n = " << n << ", beta_e = " << beta_e << ", beta_t = " << beta_t << "\n";
      errss << " dh = " << dh << ", beta0t = " << beta0t << ", beta1t = " << beta1t << "\n";
      g_log.information(errss.str());
    }

    // Reset the flag
    m_hasNewParameterValue = false;

    return;
  }

  //------------- Private Functions (Overwrite) Calculation --------------------------------------

  //----------------------------------------------------------------------------------------------
  /** Override function1D
   */
  void ThermalNeutronBk2BkExpConvPVoigt::functionLocal(double* out, const double* xValues, size_t nData) const
  {
    // 1. Calculate peak parameters
    double height = getParameter(0);

    // double d_h, tof_h, alpha, beta, H, sigma2, eta, N, gamma;
    // d_h, tof_h, eta, alpha, beta, H, sigma2, gamma, N,

    if (m_hasNewParameterValue)
      calculateParameters(false);

    double peakrange = m_fwhm*PEAKRANGE;

    // cout << "DBx212:  eta = " << eta << ", gamma = " << gamma << endl;

    double invert_sqrt2sigma = 1.0/sqrt(2.0*m_Sigma2);

    // PRAGMA_OMP(parallel for schedule(dynamic, 10))

    // PARALLEL_SET_NUM_THREADS(8);
    // PARALLEL_FOR_NO_WSP_CHECK()
    for (size_t id = 0; id < nData; ++id)
    {
      // PARALLEL_START_INTERUPT_REGION

      // a) Caclualte peak intensity
      double dT = xValues[id]-m_centre;

      double omega;
      if (fabs(dT) < peakrange)
      {
        omega = calOmega(dT, m_eta, m_N, m_Alpha, m_Beta, m_fwhm, m_Sigma2, invert_sqrt2sigma);
        omega *= height;
      }
      else
      {
        omega = 0.0;
      }

      out[id] = omega;

      /*  Disabled for parallel checking
      if (!(omega > -DBL_MAX && omega < DBL_MAX))
      {
       // Output with error
       g_log.error() << "Calcuate Peak " << mH << ", " << mK << ", " << mL << " wrong!\n";
       bool explicitoutput = true;
       calculateParameters(d_h, tof_h, eta, alpha, beta, H, sigma2, gamma, N, explicitoutput);
       calOmega(dT, eta, N, alpha, beta, H, sigma2, invert_sqrt2sigma, explicitoutput);

        out[id] = DBL_MAX;
      }
      else
      {
        out[id] = height*omega;
      }
      */

    // PARALLEL_END_INTERUPT_REGION
    } // ENDFOR data points
    // PARALLEL_CHECK_INTERUPT_REGION

    return;
  }


  //----------------------------------------------------------------------------------------------
  /** Function (local) of the vector version
    * @param out: The calculated peak intensities. This is assume to been initialized to the correct length
    * with a value of zero everywhere.
    * @param xValues: The x-values to evaluate the peak at.
   */
  void ThermalNeutronBk2BkExpConvPVoigt::function(vector<double>& out, const vector<double> &xValues) const
  {
    // calculate peak parameters
    const double HEIGHT = getParameter(0);
    const double INVERT_SQRT2SIGMA = 1.0/sqrt(2.0*m_Sigma2);

    if (m_hasNewParameterValue)
      calculateParameters(false);

    const double RANGE = m_fwhm*PEAKRANGE;

    // calculate where to start calculating
    const double LEFT_VALUE = m_centre - RANGE;
    vector<double>::const_iterator iter = std::lower_bound(xValues.begin(), xValues.end(), LEFT_VALUE);

    const double RIGHT_VALUE = m_centre + RANGE;
    vector<double>::const_iterator iter_end = std::lower_bound(iter, xValues.end(), RIGHT_VALUE);

    // 2. Calcualte
    std::size_t pos(std::distance(xValues.begin(), iter)); //second loop variable
    for ( ; iter != iter_end; ++iter)
    {
      out[pos] = HEIGHT
          * calOmega(*iter - m_centre, m_eta, m_N, m_Alpha, m_Beta, m_fwhm, m_Sigma2, INVERT_SQRT2SIGMA);
      pos++;
    } // ENDFOR data points

    return;
  }

  //----------------------------------------------------------------------------------------------
  /** Disabled derivative
  */
  void ThermalNeutronBk2BkExpConvPVoigt::functionDerivLocal(API::Jacobian* , const double* , const size_t )
  {
    throw Mantid::Kernel::Exception::NotImplementedError("functionDerivLocal is not implemented for IkedaCarpenterPV.");
  }

  /** Calculate derivative of this peak function
 */
  void ThermalNeutronBk2BkExpConvPVoigt::functionDeriv(const API::FunctionDomain& domain, API::Jacobian& jacobian)
  {
    calNumericalDeriv(domain, jacobian);
  }

  /** Get the center of the peak

  double ThermalNeutronBk2BkExpConvPVoigt::centre()const
  {
    if (m_newValueSet)
      calculateParameters(false);

    return m_centre;
  }
   */

  /** Set peak height

  void ThermalNeutronBk2BkExpConvPVoigt::setHeight(const double h)
  {
    setParameter(0, h);
    return;
  }
 */

  /** Get peak's height

  double ThermalNeutronBk2BkExpConvPVoigt::height() const
  {
    double height = this->getParameter(0);
    return height;
  }
     */

  /** Get peak's FWHM

  double ThermalNeutronBk2BkExpConvPVoigt::fwhm() const
  {
    if (m_newValueSet)
      calculateParameters(false);

    return m_fwhm;
  }
     */

  //-------------  Private Function To Calculate Peak Profile --------------------------------------------
  /** Calcualte H and eta for the peak
*/
  void ThermalNeutronBk2BkExpConvPVoigt::calHandEta(double sigma2, double gamma, double& H, double& eta) const
  {
    // 1. Calculate H
    double H_G = sqrt(8.0 * sigma2 * log(2.0));
    double H_L = gamma;

    double temp1 = std::pow(H_L, 5) + 0.07842*H_G*std::pow(H_L, 4) + 4.47163*std::pow(H_G, 2)*std::pow(H_L, 3) +
        2.42843*std::pow(H_G, 3)*std::pow(H_L, 2) + 2.69269*std::pow(H_G, 4)*H_L + std::pow(H_G, 5);

    H = std::pow(temp1, 0.2);

    // 2. Calculate eta
    double gam_pv = H_L/H;
    eta = 1.36603 * gam_pv - 0.47719 * std::pow(gam_pv, 2) + 0.11116 * std::pow(gam_pv, 3);

    if (eta > 1 || eta < 0)
    {
      g_log.warning() << "Calculated eta = " << eta << " is out of range [0, 1].\n";
    }

    return;
  }


  //----------------------------------------------------------------------------------------------
  /** Calculate Omega(x) = ... ...
 *  This is the core component to calcualte peak profile
 */
  double ThermalNeutronBk2BkExpConvPVoigt::calOmega(const double x, const double eta, const double N,
                                                    const double alpha, const double beta, const double H,
                                                    const double sigma2, const double invert_sqrt2sigma,
                                                    const bool explicitoutput) const
  {
    const double u = 0.5*alpha*(alpha*sigma2+2.*x);
    const double y = (alpha*sigma2 + x)*invert_sqrt2sigma;

    const double v = 0.5*beta*(beta*sigma2 - 2.*x);
    const double z = (beta*sigma2 - x)*invert_sqrt2sigma;

    // 2. Calculate

    const double erfcy = gsl_sf_erfc(y);
    double part1(0.);
    if (fabs(erfcy) > DBL_MIN)
      part1 = exp(u)*erfcy;

    const double erfcz = gsl_sf_erfc(z);
    double part2(0.);
    if (fabs(erfcz) > DBL_MIN)
      part2 = exp(v)*erfcz;

    const double omega1 = (1.-eta)*N*(part1 + part2);
    double omega2(0.);
    if (eta >= 1.0E-8)
    {
      const double SQRT_H_5 = sqrt(H)*.5;
      std::complex<double> p(alpha*x, alpha*SQRT_H_5);
      std::complex<double> q(-beta*x, beta*SQRT_H_5);
      double omega2a = imag(exp(p)*E1(p));
      double omega2b = imag(exp(q)*E1(q));
      omega2 = -1.0*N*eta*(omega2a + omega2b)*TWO_OVER_PI;
    }
    const double omega = omega1+omega2;

    if (explicitoutput)
    {
      if (omega <= NEG_DBL_MAX || omega >= DBL_MAX)
      {
        stringstream errss;
        errss << "Find omega = " << omega << " is infinity! omega1 = " << omega1 << ", omega2 = " << omega2 << "\n";
        errss << "  u = " << u << ", v = " << v << ", erfc(y) = " << gsl_sf_erfc(y)
              << ", erfc(z) = " << gsl_sf_erfc(z) << "\n";
        errss << "  alpha = " << alpha << ", x = " << x << " sigma2 = " << sigma2
              << ", N = " << N << "\n";
        g_log.warning(errss.str());
      }
    }

    // cout << "[DB] Final Value = " << omega << endl;
    return omega;
  }

  //----------------------------------------------------------------------------------------------
  /** Override setting parameter by parameter index
    */
  void ThermalNeutronBk2BkExpConvPVoigt::setParameter(size_t i, const double& value, bool explicitlySet)
  {
    if (i == 23)
    {
      // Lattice parameter
      if (fabs(m_unitCellSize-value) > 1.0E-8)
      {
        // If change in value is non-trivial
        m_cellParamValueChanged = true;
        ParamFunction::setParameter(i, value, explicitlySet);
        m_hasNewParameterValue = true;
        m_unitCellSize = value;
      }
    }
    else
    {
      // Non lattice parameter
      ParamFunction::setParameter(i, value, explicitlySet);
      m_hasNewParameterValue = true;
    }

    return;
  }

  //----------------------------------------------------------------------------------------------
  /** Overriding setting parameter by parameter name
    */
  void ThermalNeutronBk2BkExpConvPVoigt::setParameter(const std::string& name, const double& value, bool explicitlySet)
  {
    if (name.compare("LatticeConstant") == 0)
    {
      // Lattice parameter
      if (fabs(m_unitCellSize-value) > 1.0E-8)
      {
        // If change in value is non-trivial
        m_cellParamValueChanged = true;
        ParamFunction::setParameter(23, value, explicitlySet);
        m_hasNewParameterValue = true;
        m_unitCellSize = value;
      }
    }
    else
    {
      ParamFunction::setParameter(name, value, explicitlySet);
      m_hasNewParameterValue = true;
    }

    return;
  }

  //----------------------------------------------------------------------------------------------
  /** This is called during long-running operations,
   * and check if the algorithm has requested that it be cancelled.
  void ThermalNeutronBk2BkExpConvPVoigt::interruption_point() const
  {
    // only throw exceptions if the code is not multi threaded otherwise you contravene the OpenMP standard
    // that defines that all loops must complete, and no exception can leave an OpenMP section
    // openmp cancel handling is performed using the ??, ?? and ?? macros in each algrothim
    IF_NOT_PARALLEL
        if (m_cancel) throw Algorithm::CancelException();
  }
  */

  //-------------------------  External Functions ---------------------------------------------------
  /** Implementation of complex integral E_1
   */
  std::complex<double> E1(std::complex<double> z)
  {
    const double el = 0.5772156649015328;

    std::complex<double> exp_e1;

    double rz = real(z);
    double az = abs(z);

    if (fabs(az) < 1.0E-8)
    {
      // If z = 0, then the result is infinity... diverge!
      complex<double> r(1.0E300, 0.0);
      exp_e1 = r;
    }
    else if (az <= 10.0 || (rz < 0.0 && az < 20.0))
    {
      // Some interesting region, equal to integrate to infinity, converged
      // cout << "[DB] Type 1" << endl;

      complex<double> r(1.0, 0.0);
      exp_e1 = r;
      complex<double> cr = r;

      for (size_t k = 1; k <= 150; ++k)
      {
        double dk = double(k);
        cr = -cr * dk * z / ( (dk+1.0)*(dk+1.0) );
        exp_e1 += cr;
        if (abs(cr) < abs(exp_e1)*1.0E-15)
        {
          // cr is converged to zero
          break;
        }
      } // ENDFOR k

      // cout << "[DB] el = " << el << ", exp_e1 = " << exp_e1 << endl;

      exp_e1 = -el - log(z) + (z*exp_e1);
    }
    else
    {
      // Rest of the region
      complex<double> ct0(0.0, 0.0);
      for (int k = 120; k > 0; --k)
      {
        complex<double> dk(double(k), 0.0);
        ct0 = dk / (10.0 + dk / (z + ct0));
      } // ENDFOR k

      exp_e1 = 1.0 / (z + ct0);
      exp_e1 = exp_e1 * exp(-z);
      if (rz < 0.0 && fabs(imag(z)) < 1.0E-10 )
      {
        complex<double> u(0.0, 1.0);
        exp_e1 = exp_e1 - (PI * u);
      }
    }

    // cout << "[DB] Final exp_e1 = " << exp_e1 << "\n";

    return exp_e1;
  }

  //----------------------------------------------------------------------------------------------
  /** (Migrated from IPeakFunction)
   * General implementation of the method for all peaks. Limits the peak evaluation to
   * a certain number of FWHMs around the peak centre. The outside points are set to 0.
   * Calls functionLocal() to compute the actual values
   * @param out :: Output function values
   * @param xValues :: X values for data points
   * @param nData :: Number of data points
   */
  void ThermalNeutronBk2BkExpConvPVoigt::function1D(double* out, const double* xValues, const size_t nData)const
  {
    double c = centre();
    double dx = fabs(s_peakRadius*this->fwhm());
    int i0 = -1;
    int n = 0;
    for(size_t i = 0; i < nData; ++i)
    {
      if (fabs(xValues[i] - c) < dx)
      {
        if (i0 < 0) i0 = static_cast<int>(i);
        ++n;
      }
      else
      {
        out[i] = 0.0;
      }
    }
    if (i0 < 0 || n == 0) return;
    this->functionLocal(out+i0, xValues+i0, n);

    return;
  }

  /// Default value for the peak radius
  int ThermalNeutronBk2BkExpConvPVoigt::s_peakRadius = 5;

  //----------------------------------------------------------------------------------------------
  /** Set peak radius

  void ThermalNeutronBk2BkExpConvPVoigt::setPeakRadius(const int& r)
  {
    if (r > 0)
    {
      s_peakRadius = r;
      std::string setting = boost::lexical_cast<std::string>(r);
      Kernel::ConfigService::Instance().setString("curvefitting.peakRadius",setting);
    }
  }
    */

} // namespace CurveFitting
} // namespace Mantid
