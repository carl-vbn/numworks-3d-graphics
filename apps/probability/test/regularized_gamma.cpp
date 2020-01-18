#include <quiz.h>
#include <string.h>
#include <assert.h>
#include <float.h>
#include <cmath>
#include "../distribution/chi_squared_distribution.h"
#include "../distribution/regularized_gamma.h"

void assert_regularized_gamma_is(double s, double x, double result) {
  double r = 0.0;
  const double precision = FLT_EPSILON;
  regularizedGamma(s, x, precision, Probability::ChiSquaredDistribution::k_maxRegularizedGammaIterations, &r);
  quiz_assert(std::fabs(r - result) <= precision);
}

QUIZ_CASE(regularized_gamma) {
  assert_regularized_gamma_is(1.0, 0.0, 0.0);
  assert_regularized_gamma_is(10.0, 0.0, 0.0);
  assert_regularized_gamma_is(100.0, 0.0, 0.0);

  assert_regularized_gamma_is(1.0, INFINITY, 1.0);
  assert_regularized_gamma_is(10.0, INFINITY, 1.0);
  assert_regularized_gamma_is(100.0, INFINITY, 1.0);

  /* Choose parameters verifying both x >= s + 1.0 and x < s + 1.0 because the
   * algoritm changes.
   * To compute the result for assert_regularized_gamma_is(s, x, result),
   * compute the value of P(t<2*x) for the chi-squared distribution with 2*s degrees of
   * freedom. */

  // Chi-squared distribution with degrees of freedom = 1
  // P(x<0.5)
  assert_regularized_gamma_is(0.5, 0.25, 0.5204998778130465186819719747290946543216705322265625);
  // P(x<0.9)
  assert_regularized_gamma_is(0.5, 0.45, 0.657218288852088772244997016969136893749237060546875);
  // P(x<1.4)
  assert_regularized_gamma_is(0.5, 0.7, 0.76327642936214268321037934583728201687335968017578125);
  // P(x<2.0)
  assert_regularized_gamma_is(0.5, 1.0, 0.84270079294971486934122063508260925929606699796630290845993789783);
  // P(x<2.5)
  assert_regularized_gamma_is(0.5, 1.25, 0.88615370199334198009211149837938137352466583251953125);
  // P(x<3.0)
  assert_regularized_gamma_is(0.5, 1.5, 0.91673548333644959814508067950090636406715894480167934513599);
  // P(x<3.4)
  assert_regularized_gamma_is(0.5, 1.7, 0.9348035809218699920819517501513473689556121826171875);
  // P(x<4.6)
  assert_regularized_gamma_is(0.5, 2.3, 0.96802804382235130287170932206208817660808563232421875);
  // P(x<4.9)
  assert_regularized_gamma_is(0.5, 2.45, 0.973143304492475547107233069255016744136810302734375);
  // P(x<6.2)
  assert_regularized_gamma_is(0.5, 3.1, 0.9872249687169125653696255540126003324985504150390625);
  // P(x<6.492)
  assert_regularized_gamma_is(0.5, 3.246, 0.98916390005951970731956635063397698104381561279296875);
  // P(x<6.7)
  assert_regularized_gamma_is(0.5, 3.35, 0.9903587088715024577822987339459359645843505859375);
  // P(x<7.24)
  assert_regularized_gamma_is(0.5, 3.62, 0.99287030201052572930819906105170957744121551513671875);
  // P(x<8.26)
  assert_regularized_gamma_is(0.5, 4.13, 0.995947187204140771399352161097340285778045654296875);


  // Chi-squared distribution with degrees of freedom = 2
  // P(x<0.5)
  assert_regularized_gamma_is(1.0, 0.25, 0.22119921692859512152296019849018193781375885009765625);
  // P(x<0.9)
  assert_regularized_gamma_is(1.0, 0.45, 0.36237184837822666860773779262672178447246551513671875);
  // P(x<1.4)
  assert_regularized_gamma_is(1.0, 0.7, 0.50341469620859047306993261372554115951061248779296875);
  // P(x<2.0)
  assert_regularized_gamma_is(1.0, 1.0, 0.632120558828557665975722557050175964832305908203125);
  // P(x<2.5)
  assert_regularized_gamma_is(1.0, 1.25, 0.71349520313980985264379341970197856426239013671875);
  // P(x<3.0)
  assert_regularized_gamma_is(1.0, 1.5, 0.77686983985157020971001884390716440975666046142578125);
  // P(x<3.4)
  assert_regularized_gamma_is(1.0, 1.7, 0.8173164759472653084770854547969065606594085693359375);
  // P(x<4.6)
  assert_regularized_gamma_is(1.0, 2.3, 0.8997411562771961968110190355218946933746337890625);
  // P(x<4.9)
  assert_regularized_gamma_is(1.0, 2.45, 0.91370641350062953289778988619218580424785614013671875);
  // P(x<6.2)
  assert_regularized_gamma_is(1.0, 3.1, 0.9549507976064421921336133891600184142589569091796875);
  // P(x<6.492)
  assert_regularized_gamma_is(1.0, 3.246, 0.961070384729282967128938253154046833515167236328125);
  // P(x<6.7)
  assert_regularized_gamma_is(1.0, 3.35, 0.96491564589915501670702724368311464786529541015625);
  // P(x<7.24)
  assert_regularized_gamma_is(1.0, 3.62, 0.973217323507361786738556475029326975345611572265625);
  // P(x<8.26)
  assert_regularized_gamma_is(1.0, 4.13, 0.983917121177411591048667105496861040592193603515625);


  // Chi-squared distribution with degrees of freedom = 3
  // P(x<0.5)
  assert_regularized_gamma_is(1.5, 0.25, 0.081108588345324139634584525992977432906627655029296875);
  // P(x<0.9)
  assert_regularized_gamma_is(1.5, 0.45, 0.174572190958339257260689691975130699574947357177734375);
  // P(x<1.4)
  assert_regularized_gamma_is(1.5, 0.7, 0.294465268795908785559589659897028468549251556396484375);
  // P(x<2.0)
  assert_regularized_gamma_is(1.5, 1.0, 0.42759329552912017646804088144563138484954833984375);
  // P(x<2.5)
  assert_regularized_gamma_is(1.5, 1.25, 0.5247089166569793849959069120814092457294464111328125);
  // P(x<3.0)
  assert_regularized_gamma_is(1.5, 1.5, 0.6083748237289110516456958066555671393871307373046875);
  // P(x<3.4)
  assert_regularized_gamma_is(1.5, 1.7, 0.666034750909839434740433716797269880771636962890625);
  // P(x<4.6)
  assert_regularized_gamma_is(1.5, 2.3, 0.79645791832063472792668790134484879672527313232421875);
  // P(x<4.9)
  assert_regularized_gamma_is(1.5, 2.45, 0.82073219631565053067134840603102929890155792236328125);
  // P(x<6.2)
  assert_regularized_gamma_is(1.5, 3.1, 0.89772497345306245808416178988409228622913360595703125);
  // P(x<6.492)
  assert_regularized_gamma_is(1.5, 3.246, 0.91002146130651773159314643635298125445842742919921875);
  // P(x<6.7)
  assert_regularized_gamma_is(1.5, 3.35, 0.91789994038620170702102996074245311319828033447265625);
  // P(x<7.24)
  assert_regularized_gamma_is(1.5, 3.62, 0.935370901057762438313147868029773235321044921875);
  // P(x<8.26)
  assert_regularized_gamma_is(1.5, 4.13, 0.95906693562948053255468039424158632755279541015625);
}

