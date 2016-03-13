#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

using namespace std;
using namespace cv;

static const double MSSIM_THRESHOLD = 0.6;

// Copied from http://docs.opencv.org/2.4/doc/tutorials/highgui/video-input-psnr-ssim/video-input-psnr-ssim.html
Scalar GetMSSIM( const Mat& i1, const Mat& i2)
{
  const double C1 = 6.5025, C2 = 58.5225;
  /***************************** INITS **********************************/
  int d = CV_32F;

  Mat I1, I2;
  i1.convertTo(I1, d);            // cannot calculate on one byte large values
  i2.convertTo(I2, d);

  Mat I2_2   = I2.mul(I2);        // I2^2
  Mat I1_2   = I1.mul(I1);        // I1^2
  Mat I1_I2  = I1.mul(I2);        // I1 * I2

  /*************************** END INITS **********************************/

  Mat mu1, mu2;                   // PRELIMINARY COMPUTING
  GaussianBlur(I1, mu1, Size(11, 11), 1.5);
  GaussianBlur(I2, mu2, Size(11, 11), 1.5);

  Mat mu1_2   =   mu1.mul(mu1);
  Mat mu2_2   =   mu2.mul(mu2);
  Mat mu1_mu2 =   mu1.mul(mu2);

  Mat sigma1_2, sigma2_2, sigma12;

  GaussianBlur(I1_2, sigma1_2, Size(11, 11), 1.5);
  sigma1_2 -= mu1_2;

  GaussianBlur(I2_2, sigma2_2, Size(11, 11), 1.5);
  sigma2_2 -= mu2_2;

  GaussianBlur(I1_I2, sigma12, Size(11, 11), 1.5);
  sigma12 -= mu1_mu2;

  ///////////////////////////////// FORMULA ////////////////////////////////
  Mat t1, t2, t3;

  t1 = 2 * mu1_mu2 + C1;
  t2 = 2 * sigma12 + C2;
  t3 = t1.mul(t2);                 // t3 = ((2*mu1_mu2 + C1).*(2*sigma12 + C2))

  t1 = mu1_2 + mu2_2 + C1;
  t2 = sigma1_2 + sigma2_2 + C2;
  t1 = t1.mul(t2);                 // t1 =((mu1_2 + mu2_2 + C1).*(sigma1_2 + sigma2_2 + C2))

  Mat ssim_map;
  divide(t3, t1, ssim_map);        // ssim_map =  t3./t1;

  Scalar mssim = mean(ssim_map);   // mssim = average of ssim map
  return mssim;
}

bool ContainsTemplate(Mat& input, Mat& templ, double threshold = MSSIM_THRESHOLD) {
    // Convert to grayscale
    Mat gray_input, gray_templ;
    cvtColor(input, gray_input, COLOR_BGR2GRAY);
    cvtColor(templ, gray_templ, COLOR_BGR2GRAY);

    // Create the result matrix
    int result_cols = input.cols - templ.cols + 1;
    int result_rows = input.rows - templ.rows + 1;
    Mat result(result_cols, result_rows, CV_32FC1);

    // Do the Matching and Normalize
    matchTemplate(input, templ, result, CV_TM_CCOEFF);
    normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

    Point maxLoc;
    minMaxLoc(result, NULL, NULL, NULL, &maxLoc);
    Rect bounds(maxLoc.x, maxLoc.y, templ.cols, templ.rows);
    Mat cropped = input(bounds);
    double similarity = GetMSSIM(cropped, templ)[0];
    //cout << "MSSIM: " << similarity << endl;

    return similarity > threshold;
}

bool IsWinScreen(Mat& screen) {
  Mat templ = imread("img/wins_template.jpg");
  return ContainsTemplate(screen, templ);
}

class CharacterDetails {
public:
  CharacterDetails(string _name, string _template_filename): name(_name), template_filename(_template_filename) {}
  string name;
  string template_filename;
};

static const CharacterDetails FALCON("Falcon", "img/cfalcon_template.jpg");
static const CharacterDetails DK("DK", "img/dk_template.jpg");
static const CharacterDetails FOX("Fox", "img/fox_template.jpg");
static const CharacterDetails JIGGLYPUFF("Jigglypuff", "img/jigglypuff_template.jpg");
static const CharacterDetails KIRBY("Kirby", "img/kirby_template.jpg");
static const CharacterDetails LINK("Link", "img/link_template.jpg");
static const CharacterDetails MARIO("Mario", "img/mario_template.jpg");
static const CharacterDetails NESS("Ness", "img/ness_template.jpg");
static const CharacterDetails PIKACHU("Pikachu", "img/pikachu_template.jpg");
static const CharacterDetails SAMUS("Samus", "img/samus_template.jpg");
static const CharacterDetails YOSHI("Yoshi", "img/yoshi_template.jpg");
static const CharacterDetails CHARACTERS[] = {
  FALCON,
  DK,
  FOX,
  JIGGLYPUFF,
  KIRBY,
  LINK,
  MARIO,
  NESS,
  PIKACHU,
  SAMUS,
  YOSHI,
};

bool GetCharacterMatch(Mat& screen, string& out_character) {
  for (CharacterDetails character: CHARACTERS) {
    Mat templ = imread(character.template_filename);
    if (ContainsTemplate(screen, templ)) {
      out_character = character.name;
      return true;
    }
  }
  return false;
}

// for testing
int main(int argc, char* argv[])
{
  string filenames[] = {
    "img/cfalcon.png",
    "img/dk.png",
    "img/fox.png",
    "img/jigglypuff.png",
    "img/kirby.png",
    "img/link.png",
    "img/mario.png",
    "img/ness.png",
    "img/pikachu.png",
    "img/samus.png",
    "img/yoshi.png",
    "img/character_select.png",
    "img/stage_select.png",
    "img/fight_1.png",
    "img/fight_2.png",
    "img/fight_3.png",
    "img/fight_4.png",
  };

  // Check files
  cout << "Running tests..." << endl;
  cout << "filename, IsWinScreen, GetCharacterMatch, Name" << endl;
  for (string filename: filenames) {
    Mat input = imread(filename);
    bool is_win = false, is_character_match = false;
    string character = "n/a";
    is_win = IsWinScreen(input);
    if (is_win) {
      is_character_match = GetCharacterMatch(input, character);
    }
    cout << filename << ": " << is_win << ", " << is_character_match << ", " << character << endl;
  }
  cout << "Done" << endl;
  return 0;
}

// for command line use
int main2(int argc, char* argv[])
{

    cout << endl << argv[0] << endl << endl;
    cout << "Analyzes an image to determine if it is a SSB64 win screen, and if so, which character is the winner." << endl;

    Mat input;
    if (argc > 1) {
      input = imread(argv[1]);
    } else {
      cout << "    Usage: " << argv[0] << " <input_image>" << endl;
      return 0;
    }
    Mat templ = imread("img/wins_template.jpg");
    bool matched = ContainsTemplate(input, templ);
    return 0;
}
