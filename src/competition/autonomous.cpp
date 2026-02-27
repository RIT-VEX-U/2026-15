#include "competition/autonomous.h"

#include "core/utils/command_structure/auto_command.h"
#include "core/utils/command_structure/command_controller.h"
#include "core/utils/command_structure/delay_command.h"
#include "core/utils/command_structure/drive_commands.h"
#include "core/utils/pure_pursuit.h"
#include "robot-config.h"
#include "subsystems/superstructure.h"

#include "subsystems/macros.h"


void red_left();
void red_right();

void skills();

void autonomous() {
    red_left();
}

PurePursuit::Path left_wing_path() {
    std::vector<Translation2d> pathPoints = {
        Translation2d(23.750, 118.750),
        Translation2d(25.328, 118.761),
        Translation2d(26.705, 118.793),
        Translation2d(27.900, 118.848),
        Translation2d(28.933, 118.928),
        Translation2d(29.823, 119.032),
        Translation2d(30.590, 119.163),
        Translation2d(31.252, 119.321),
        Translation2d(31.830, 119.508),
        Translation2d(32.343, 119.724),
        Translation2d(32.810, 119.971),
        Translation2d(33.251, 120.250),
        Translation2d(33.685, 120.562),
        Translation2d(34.132, 120.908),
        Translation2d(34.611, 121.290),
        Translation2d(35.142, 121.707),
        Translation2d(35.743, 122.162),
        Translation2d(36.435, 122.656),
        Translation2d(37.237, 123.190),
        Translation2d(38.168, 123.764),
        Translation2d(39.248, 124.380),
        Translation2d(40.375, 124.983),
        Translation2d(41.441, 125.521),
        Translation2d(42.451, 125.996),
        Translation2d(43.414, 126.412),
        Translation2d(44.337, 126.775),
        Translation2d(45.226, 127.086),
        Translation2d(46.089, 127.350),
        Translation2d(46.934, 127.571),
        Translation2d(47.766, 127.753),
        Translation2d(48.595, 127.899),
        Translation2d(49.426, 128.013),
        Translation2d(50.267, 128.099),
        Translation2d(51.125, 128.160),
        Translation2d(52.007, 128.202),
        Translation2d(52.921, 128.226),
        Translation2d(53.873, 128.238),
        Translation2d(54.872, 128.240),
        Translation2d(55.923, 128.237),
        Translation2d(57.035, 128.232),
        Translation2d(58.214, 128.230),
        Translation2d(58.514, 128.230),
        Translation2d(58.814, 128.230),
        Translation2d(59.114, 128.230),
        Translation2d(59.414, 128.230),
        Translation2d(59.714, 128.230),
    };

    return PurePursuit::Path(pathPoints, 8.000);
}

// PurePursuit::Path buildPath() {
//     std::vector<PurePursuit::hermite_point> hermitePoints = {
//         PurePursuit::hermite_point{23.750, 23.750, 0.000, 35.000},
//         PurePursuit::hermite_point{38.356, 30.664, 0.349, 32.764},
//         PurePursuit::hermite_point{58.214, 33.230, 0.000, 20.925},
//     };
//
//     std::vector<Translation2d> smoothed = PurePursuit::smooth_path_hermite(hermitePoints, 20.000);
//     return PurePursuit::Path(smoothed, 8.000);
// }

PurePursuit::Path right_wing_path() {
    std::vector<Translation2d> pathPoints = {
        Translation2d(23.750, 23.750),
        Translation2d(25.362, 23.773),
        Translation2d(26.717, 23.843),
        Translation2d(27.842, 23.956),
        Translation2d(28.764, 24.110),
        Translation2d(29.511, 24.305),
        Translation2d(30.110, 24.537),
        Translation2d(30.590, 24.805),
        Translation2d(30.976, 25.107),
        Translation2d(31.297, 25.442),
        Translation2d(31.580, 25.806),
        Translation2d(31.852, 26.198),
        Translation2d(32.141, 26.616),
        Translation2d(32.475, 27.058),
        Translation2d(32.880, 27.523),
        Translation2d(33.385, 28.007),
        Translation2d(34.016, 28.510),
        Translation2d(34.802, 29.029),
        Translation2d(35.768, 29.562),
        Translation2d(36.944, 30.108),
        Translation2d(38.356, 30.664),
        Translation2d(39.839, 31.189),
        Translation2d(41.217, 31.644),
        Translation2d(42.498, 32.035),
        Translation2d(43.692, 32.366),
        Translation2d(44.807, 32.642),
        Translation2d(45.852, 32.867),
        Translation2d(46.837, 33.045),
        Translation2d(47.770, 33.182),
        Translation2d(48.661, 33.281),
        Translation2d(49.518, 33.348),
        Translation2d(50.350, 33.387),
        Translation2d(51.166, 33.403),
        Translation2d(51.976, 33.400),
        Translation2d(52.788, 33.382),
        Translation2d(53.612, 33.354),
        Translation2d(54.456, 33.322),
        Translation2d(55.329, 33.288),
        Translation2d(56.241, 33.259),
        Translation2d(57.199, 33.238),
        Translation2d(58.214, 33.230),
        Translation2d(58.514, 33.230),
        Translation2d(58.814, 33.230),
        Translation2d(59.114, 33.230),
        Translation2d(59.414, 33.230),
        Translation2d(59.714, 33.230),
    };

    return PurePursuit::Path(pathPoints, 7.000);
}
void red_left() {
    CommandController cc{
      LoadDownCmd(),
      drive_sys.DriveToPointCmd({20, 119}, vex::forward, 1)->withTimeout(1.5),
      IntakeCmd(),
      drive_sys.TurnToHeadingCmd(180, 1)->withTimeout(1),
      DriveTankRawCmd(0.4, 0.4),

      // only intake long enough for reds
      new DelayCommand(1300),
      (new Parallel({
        drive_sys.DriveToPointCmd({42, 118.75}, vex::reverse, 1)->withTimeout(2),
        new InOrder({new DelayCommand(100), LoadUpCmd(), LiftUpCmd(), IntakeStopCmd()}),
      }))->withTimeout(1.5),
      ScoreUpperCmd(),
      new DelayCommand(100),

      // drive_sys.DriveForwardCmd(18, vex::forward, 1)->withTimeout(1),
      drive_sys.DriveToPointCmd({24, 118.75}, vex::forward, 1)->withTimeout(1),
      WingDownCmd(),
      

      // DriveTankRawCmd(0.2, 0.4),
      // new DelayCommand(200),
      // DriveTankRawCmd(0.4, 0.2),
      // new DelayCommand(200),
      // drive_sys.DriveToPointCmd({52, 34}, vex::reverse, 0.8)->withTimeout(2),
      // drive_sys.DriveToPointCmd({61, 34}, vex::reverse, 1)->withTimeout(3),
      drive_sys.PurePursuitCmd(left_wing_path(), vex::reverse, 0.5, 0.5)->withTimeout(3),

      DriveTankRawCmd(-0.2, -0.2),
      new DelayCommand(250),
      DriveTankRawCmd(0, 0),


      BrakeDriveCmd(vex::brakeType::hold),
      





    };
    cc.run();
}



void red_right() {
    CommandController cc{
      LoadDownCmd(),
      drive_sys.DriveToPointCmd({20, 22.5}, vex::forward, 1)->withTimeout(1.5),
      IntakeCmd(),
      drive_sys.TurnToHeadingCmd(180, 1)->withTimeout(1),
      DriveTankRawCmd(0.4, 0.4),

      // only intake long enough for reds
      new DelayCommand(1300),
      (new Parallel({
        drive_sys.DriveToPointCmd({42, 23.75}, vex::reverse, 1)->withTimeout(2),
        new InOrder({new DelayCommand(100), LoadUpCmd(), LiftUpCmd(), IntakeStopCmd()}),
      }))->withTimeout(1.5),
      ScoreUpperCmd(),
      new DelayCommand(100),

      // drive_sys.DriveForwardCmd(18, vex::forward, 1)->withTimeout(1),
      drive_sys.DriveToPointCmd({24, 23.75}, vex::forward, 1)->withTimeout(1),
      WingDownCmd(),
      

      // DriveTankRawCmd(0.2, 0.4),
      // new DelayCommand(200),
      // DriveTankRawCmd(0.4, 0.2),
      // new DelayCommand(200),
      // drive_sys.DriveToPointCmd({52, 34}, vex::reverse, 0.8)->withTimeout(2),
      // drive_sys.DriveToPointCmd({61, 34}, vex::reverse, 1)->withTimeout(3),
      drive_sys.PurePursuitCmd(right_wing_path(), vex::reverse, 0.5, 0.5)->withTimeout(3),

      
      DriveTankRawCmd(-0.2, -0.2),
      new DelayCommand(250),
      DriveTankRawCmd(0, 0),
      BrakeDriveCmd(vex::brakeType::hold),





    };
    cc.run();
}

void skills() {
    CommandController cc{

    };
    cc.run();
}
