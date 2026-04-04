#include "competition/autonomous.h"

#include <cmath>
#include <vector>

#include "core/units/units.h"
#include "core/utils/command_structure/auto_command.h"
#include "core/utils/command_structure/command_controller.h"
#include "core/utils/command_structure/delay_command.h"
#include "core/utils/command_structure/drive_commands.h"
#include "core/utils/math/geometry/rotation2d.h"
#include "core/utils/trajectory/constraints/centripetal_acceleration_constraint.h"
#include "core/utils/trajectory/constraints/tank_voltage_constraint.h"
#include "core/utils/trajectory/trajectory_generator.h"
#include "robot-config.h"
#include "subsystems/macros.h"
#include "subsystems/superstructure.h"

void red_left();
void red_right();
void awp_right();

void skills();
void skills_longer();
void skills_slow();

void autonomous() {
    // skills();
    // skills_longer();
    skills_slow();
    // red_left();
    // red_right();
    // awp_right();
}

Trajectory make_example_curve() {
  

  constexpr double radius = 24.0;
  constexpr double cx = 71.25;
  constexpr double cy = 71.0;
  constexpr double tangent_mag = radius * M_PI / 4.0;
  constexpr double diag = 16.97056274847714;

  std::vector<HermitePoint> points = {
    HermitePoint::from_heading(cx, cy + radius, 0.0, tangent_mag),
    HermitePoint::from_heading(cx + diag, cy + diag, -M_PI_4, tangent_mag),
    HermitePoint::from_heading(cx + radius, cy, -M_PI_2, tangent_mag),
    HermitePoint::from_heading(cx + diag, cy - diag, -3.0 * M_PI_4, tangent_mag),
    HermitePoint::from_heading(cx, cy - radius, M_PI, tangent_mag),
    HermitePoint::from_heading(cx - diag, cy - diag, 3.0 * M_PI_4, tangent_mag),
    HermitePoint::from_heading(cx - radius, cy, M_PI_2, tangent_mag),
    HermitePoint::from_heading(cx - diag, cy + diag, M_PI_4, tangent_mag),
    HermitePoint::from_heading(cx, cy + radius, 0.0, tangent_mag),
  };

  TrajectoryConfig config(60.000_inps, 60.000_inps2);
  config.set_start_velocity(0.000_inps);
  config.set_end_velocity(0.000_inps);
  config.set_reversed(false);
  config.set_track_width(12.000_in);
  config.add_constraint(CentripetalAccelerationConstraint(120.000_inps2));
  config.add_constraint(TankVoltageConstraint(0.118581_VpInps, 0.016758_VpInps2, 12.000_V, 12.000_in));
  return TrajectoryGenerator::generate_trajectory(points, config);
}

Trajectory left_drive_to_blue() {
  

  std::vector<HermitePoint> points = {
    HermitePoint::from_heading(23.750, 118.000, M_PI / 4.0, 17.000),
    {45.000, 130.000, 30.000, 0.000},
    {90.000, 130.000, 40.000, 0.000},
    {118.000, 118.000, 20.000, 0.000},
  };

  TrajectoryConfig config(60.000_inps, 60.000_inps2);
  config.set_start_velocity(0.000_inps);
  config.set_end_velocity(0.000_inps);
  config.set_reversed(true);
  config.set_track_width(12.000_in);
  config.add_constraint(CentripetalAccelerationConstraint(120.000_inps2));
  config.add_constraint(TankVoltageConstraint(0.118581_VpInps, 0.016758_VpInps2, 12.000_V, 12.000_in));
  return TrajectoryGenerator::generate_trajectory(points, config);
}

Trajectory left_blue_to_red() {
  

  std::vector<HermitePoint> points = {
    {119.286, 118.508, -15.000, 15.000},
    {95.832, 129.500, -42.000, 0.000},
    {48.640, 129.500, -42.000, 0.000},
    {24.000, 118.000, -22.000, -22.000},
    {10.000, 100.000, 0.000, -15.000},
    {10.000, 88.000, 0.000, -13.000},
  };

  TrajectoryConfig config(40.000_inps, 30.000_inps2);
  config.set_start_velocity(0.000_inps);
  config.set_end_velocity(0.000_inps);
  config.set_reversed(true);
  config.set_track_width(12.000_in);
  config.add_constraint(CentripetalAccelerationConstraint(120.000_inps2));
  config.add_constraint(TankVoltageConstraint(0.118581_VpInps, 0.016758_VpInps2, 12.000_V, 12.000_in));
  return TrajectoryGenerator::generate_trajectory(points, config);
}

Trajectory left_wing() {
  

  std::vector<HermitePoint> points = {
    {24.000, 118.000, 25.000, 25.000},
    {40.000, 128.500, 10.000, 0.000},
    {61.000, 128.500, 30.000, 0.000},
  };

  TrajectoryConfig config(60.000_inps, 60.000_inps2);
  config.set_start_velocity(0.000_inps);
  config.set_end_velocity(0.000_inps);
  config.set_reversed(true);
  config.set_track_width(12.000_in);
  config.add_constraint(CentripetalAccelerationConstraint(120.000_inps2));
  config.add_constraint(TankVoltageConstraint(0.118581_VpInps, 0.016758_VpInps2, 12.000_V, 12.000_in));
  return TrajectoryGenerator::generate_trajectory(points, config);
}

Trajectory right_wing() {
  

  std::vector<HermitePoint> points = {
    {24.000, 24.000, 28.000, 28.000},
    {39.000, 33.500, 10.000, 0.000},
    {61.000, 33.500, 30.000, 0.000},
  };

  TrajectoryConfig config(40.000_inps, 30.000_inps2);
  config.set_start_velocity(0.000_inps);
  config.set_end_velocity(0.000_inps);
  config.set_reversed(true);
  config.set_track_width(12.000_in);
  config.add_constraint(CentripetalAccelerationConstraint(120.000_inps2));
  config.add_constraint(TankVoltageConstraint(0.118581_VpInps, 0.016758_VpInps2, 12.000_V, 12.000_in));
  return TrajectoryGenerator::generate_trajectory(points, config);
}

Trajectory left_to_loader() {
  

  std::vector<HermitePoint> points = {
    {18.000, 92.000, 0.000, 40.000},
    {18.000, 117.000, -60.000, 0.000},
  };

  TrajectoryConfig config(60.000_inps, 60.000_inps2);
  config.set_start_velocity(0.000_inps);
  config.set_end_velocity(0.000_inps);
  config.set_reversed(false);
  config.set_track_width(12.000_in);
  config.add_constraint(CentripetalAccelerationConstraint(120.000_inps2));
  config.add_constraint(TankVoltageConstraint(0.118581_VpInps, 0.016758_VpInps2, 12.000_V, 12.000_in));
  return TrajectoryGenerator::generate_trajectory(points, config);
}

Trajectory right_to_loader() {
  

  std::vector<HermitePoint> points = {
    {18.000, 54.000, 0.000, -40.000},
    {18.000, 25.000, -60.000, 0.000},
  };

  TrajectoryConfig config(60.000_inps, 80.000_inps2);
  config.set_start_velocity(0.000_inps);
  config.set_end_velocity(0.000_inps);
  config.set_reversed(false);
  config.set_track_width(12.000_in);
  config.add_constraint(CentripetalAccelerationConstraint(120.000_inps2));
  config.add_constraint(TankVoltageConstraint(0.118581_VpInps, 0.016758_VpInps2, 12.000_V, 12.000_in));
  return TrajectoryGenerator::generate_trajectory(points, config);
}

Trajectory loader_to_lower_middle() {
  

  std::vector<HermitePoint> points = {
    {24.000, 24.000, -50.000, 30.000},
    {58.000, 60.000, 20.000, 20.000},
  };

  TrajectoryConfig config(60.000_inps, 80.000_inps2);
  config.set_start_velocity(0.000_inps);
  config.set_end_velocity(0.000_inps);
  config.set_reversed(false);
  config.set_track_width(12.000_in);
  config.add_constraint(CentripetalAccelerationConstraint(120.000_inps2));
  config.add_constraint(TankVoltageConstraint(0.118581_VpInps, 0.016758_VpInps2, 12.000_V, 12.000_in));
  return TrajectoryGenerator::generate_trajectory(points, config);
}

Trajectory lower_middle_to_wing() {
  

  std::vector<HermitePoint> points = {
    {56.000, 56.000, -20.000, -20.000},
    {38.000, 42.000, -10.000, -10.000},
    {40.000, 33.000, 10.000, 0.000},
    {61.000, 33.000, 20.000, 0.000},
  };

  TrajectoryConfig config(60.000_inps, 80.000_inps2);
  config.set_start_velocity(0.000_inps);
  config.set_end_velocity(0.000_inps);
  config.set_reversed(true);
  config.set_track_width(12.000_in);
  config.add_constraint(CentripetalAccelerationConstraint(120.000_inps2));
  config.add_constraint(TankVoltageConstraint(0.118581_VpInps, 0.016758_VpInps2, 12.000_V, 12.000_in));
  return TrajectoryGenerator::generate_trajectory(points, config);
}

Trajectory right_to_left_far_side_skills() {
  

  std::vector<HermitePoint> points = {
    {118.000, 118.000, 0.000, -50.000},
    {105.000, 71.250, -12.810, -47.333},
    {120.000, 22.000, 50.000, 0.000},
  };

  TrajectoryConfig config(60.000_inps, 60.000_inps2);
  config.set_start_velocity(0.000_inps);
  config.set_end_velocity(0.000_inps);
  config.set_reversed(false);
  config.set_track_width(12.000_in);
  config.add_constraint(CentripetalAccelerationConstraint(120.000_inps2));
  config.add_constraint(TankVoltageConstraint(0.118581_VpInps, 0.016758_VpInps2, 12.000_V, 12.000_in));
  return TrajectoryGenerator::generate_trajectory(points, config);
}

Trajectory far_side_loader_to_middle() {
  

  std::vector<HermitePoint> points = {
    {128.000, 15.000, -50.000, 0.000},
    {81.000, 60.000, -20.000, 20.000},
  };

  TrajectoryConfig config(60.000_inps, 60.000_inps2);
  config.set_start_velocity(0.000_inps);
  config.set_end_velocity(0.000_inps);
  config.set_reversed(true);
  config.set_track_width(12.000_in);
  config.add_constraint(CentripetalAccelerationConstraint(120.000_inps2));
  config.add_constraint(TankVoltageConstraint(0.118581_VpInps, 0.016758_VpInps2, 12.000_V, 12.000_in));
  return TrajectoryGenerator::generate_trajectory(points, config);
}

Trajectory return_to_far_left() {
  

  std::vector<HermitePoint> points = {
    {99.661, 42.164, 23.544, 44.845},
    {118.000, 118.000, -20.000, 20.000},
  };

  TrajectoryConfig config(60.000_inps, 60.000_inps2);
  config.set_start_velocity(0.000_inps);
  config.set_end_velocity(0.000_inps);
  config.set_reversed(false);
  config.set_track_width(12.000_in);
  config.add_constraint(CentripetalAccelerationConstraint(120.000_inps2));
  config.add_constraint(TankVoltageConstraint(0.118581_VpInps, 0.016758_VpInps2, 12.000_V, 12.000_in));
  return TrajectoryGenerator::generate_trajectory(points, config);
}

void run_trajectory_test() {
    Trajectory trajectory = make_example_curve();
    drive_sys.reset_auto();
    vexDelay(20);

    CommandController cc{
      drive_sys.FollowTrajectoryCmd(trajectory, trajectory_follower_config)->withTimeout(10),
    };
    cc.run();
}

void red_left() {
    printf("red left\n");

    Trajectory traj_left_to_loader = left_to_loader();
    Trajectory traj_left_wing = left_wing();

    CommandController cc{
      LoadDownCmd(),
      IntakeCmd(),
      drive_sys.FollowTrajectoryCmd(traj_left_to_loader, trajectory_follower_config)->withTimeout(10),

      DriveTankRawCmd(0.35, 0.35),
      new DelayCommand(1100),
      DriveTankRawCmd(0, 0),
      new DelayCommand(200),
      (new Parallel({
        drive_sys.DriveToPointCmd({42, 118}, vex::reverse, 1)->withTimeout(2),
        new InOrder({new DelayCommand(100), LoadUpCmd(), LiftUpCmd(), HoodOpenCmd()}),
      }))->withTimeout(1.5),
      DriveTankRawCmd(-0.2, -0.2),
      ScoreUpperCmd(),
      new DelayCommand(100),
      IntakeStopCmd(),

      drive_sys.DriveToPointCmd({24, 118}, vex::forward, 1)->withTimeout(1),
      WingDownCmd(),

      drive_sys.FollowTrajectoryCmd(traj_left_wing, trajectory_follower_config)->withTimeout(10),

      BrakeDriveCmd(vex::brakeType::hold),
    };
    cc.run();
}

void awp_right() {
    printf("awp right\n");

    TankTrajectoryFollowerConfig skills_config = trajectory_follower_config;
    skills_config.q_tolerances = {0.5, 0.5, 1, 1, 1};

    Trajectory traj_right_to_loader = right_to_loader();
    Trajectory traj_loader_to_lower_middle = loader_to_lower_middle();
    Trajectory traj_lower_middle_to_wing = lower_middle_to_wing();

    CommandController cc{
      LoadDownCmd(),
      IntakeCmd(),
      drive_sys.FollowTrajectoryCmd(traj_right_to_loader, skills_config)->withTimeout(10),

      DriveTankRawCmd(0.35, 0.35),
      new DelayCommand(1200),
      DriveTankRawCmd(0, 0),
      new DelayCommand(200),
      (new Parallel({
        drive_sys.DriveToPointCmd({42, 23.75}, vex::reverse, 1)->withTimeout(2),
        new InOrder({new DelayCommand(100), LoadUpCmd(), LiftUpCmd(), HoodOpenCmd()}),
      }))->withTimeout(1.5),
      DriveTankRawCmd(-0.2, -0.2),
      ScoreUpperCmd(45),
      new DelayCommand(100),
      IntakeStopCmd(),
      odom.SetPositionCmd(Pose2d(45, 23.75, from_degrees(180))),
      new DelayCommand(750),

      drive_sys.DriveToPointCmd({28, 23.75}, vex::forward, 1)->withTimeout(1),
      drive_sys.TurnToHeadingCmd(Rotation2d(-60, 30).wrapped_degrees_360(), 0.5)->withTimeout(1),

      drive_sys.FollowTrajectoryCmd(traj_loader_to_lower_middle, skills_config)->withTimeout(10),
      OuttakeCmd(),
      new DelayCommand(1000),
      IntakeStopCmd(),

      WingDownCmd(),
      drive_sys.FollowTrajectoryCmd(traj_lower_middle_to_wing, skills_config)->withTimeout(10),

      BrakeDriveCmd(vex::brakeType::hold),
    };
    cc.run();
}

void red_right() {
    printf("red left\n");

    TankTrajectoryFollowerConfig skills_config = trajectory_follower_config;
    skills_config.q_tolerances = {0.5, 0.5, 0.9, 1, 1};

    Trajectory traj_right_to_loader = right_to_loader();
    Trajectory traj_right_wing = right_wing();

    CommandController cc{
      LoadDownCmd(),
      IntakeCmd(),
      drive_sys.FollowTrajectoryCmd(traj_right_to_loader, skills_config)->withTimeout(10),

      DriveTankRawCmd(0.35, 0.35),
      new DelayCommand(1100),
      DriveTankRawCmd(0, 0),
      new DelayCommand(200),
      (new Parallel({
        drive_sys.DriveToPointCmd({42, 23.75}, vex::reverse, 1)->withTimeout(2),
        new InOrder({new DelayCommand(100), LoadUpCmd(), LiftUpCmd(), HoodOpenCmd()}),
      }))->withTimeout(1.5),
      DriveTankRawCmd(-0.2, -0.2),
      ScoreUpperCmd(),
      new DelayCommand(100),
      IntakeStopCmd(),

      drive_sys.DriveToPointCmd({22, 22}, vex::forward, 1)->withTimeout(1),
      WingDownCmd(),
      drive_sys.FollowTrajectoryCmd(traj_right_wing, skills_config)->withTimeout(10),

      BrakeDriveCmd(vex::brakeType::hold),
    };
    cc.run();
}

void skills() {
    TankTrajectoryFollowerConfig skills_config = trajectory_follower_config;
    skills_config.q_tolerances = {0.5, 0.5, 1, 1, 1};

    Trajectory traj_left_to_loader = left_to_loader();
    Trajectory traj_left_drive_to_blue = left_drive_to_blue();
    Trajectory traj_left_blue_to_red = left_blue_to_red();

    CommandController cc{
      LoadDownCmd(),
      IntakeCmd(),
      drive_sys.FollowTrajectoryCmd(traj_left_to_loader, skills_config)->withTimeout(10),

      DriveTankRawCmd(0.4, 0.4),
      new DelayCommand(4000),
      (new Parallel({
        drive_sys.DriveToPointCmd({24, 118}, vex::reverse, 1)->withTimeout(2),
        new InOrder({new DelayCommand(100), LoadUpCmd(), LiftUpCmd(), IntakeStopCmd()}),
      }))->withTimeout(1.5),

      drive_sys.TurnToHeadingCmd(-135, 0.6)->withTimeout(1),
      drive_sys.FollowTrajectoryCmd(traj_left_drive_to_blue, skills_config)->withTimeout(10),

      drive_sys.TurnToHeadingCmd(0, 0.6)->withTimeout(1),
      HoodOpenCmd(),
      drive_sys.DriveToPointCmd({102, 118}, vex::reverse, 1)->withTimeout(2),
      DriveTankRawCmd(-0.3, -0.3),
      new DelayCommand(1000),
      ScoreUpperCmd(),
      new DelayCommand(500),

      drive_sys.DriveToPointCmd({124, 117}, vex::forward, 1)->withTimeout(2),
      LoadDownCmd(),
      IntakeCmd(),
      new DelayCommand(500),

      DriveTankRawCmd(0.40, 0.40),
      new DelayCommand(4000),

      (new Parallel({
        drive_sys.DriveToPointCmd({102, 118}, vex::reverse, 1)->withTimeout(2),
        new InOrder({new DelayCommand(100), LoadUpCmd(), LiftUpCmd(), IntakeStopCmd(), HoodOpenCmd()}),
      }))->withTimeout(1.5),
      DriveTankRawCmd(-0.3, -0.3),
      LoadUpCmd(),
      new DelayCommand(250),
      HoodOpenCmd(),

      new DelayCommand(750),
      ScoreUpperSlowCmd(),
      new DelayCommand(1000),

      DriveTankRawCmd(0.3, 0.3),
      new DelayCommand(500),
      DriveTankRawCmd(0, 0),
      drive_sys.DriveToPointCmd({118, 118}, vex::forward, 0.5)->withTimeout(5),
      drive_sys.TurnToHeadingCmd(-45, 0.6)->withTimeout(1),

      drive_sys.FollowTrajectoryCmd(traj_left_blue_to_red, skills_config)->withTimeout(10),

      DriveTankRawCmd(-0.35, -0.35),
    };
    cc.run();
}

void skills_longer() {
    TankTrajectoryFollowerConfig skills_config = trajectory_follower_config;
    skills_config.q_tolerances = {0.5, 0.5, 1, 1, 1};

    Trajectory traj_left_to_loader = left_to_loader();
    Trajectory traj_left_drive_to_blue = left_drive_to_blue();
    Trajectory traj_right_to_left_far_side = right_to_left_far_side_skills();
    Trajectory traj_far_side_loader_to_middle = far_side_loader_to_middle();
    Trajectory traj_return_to_far_left = return_to_far_left();
    Trajectory traj_left_blue_to_red = left_blue_to_red();

    CommandController cc{
      LoadDownCmd(),
      IntakeCmd(),
      drive_sys.FollowTrajectoryCmd(traj_left_to_loader, skills_config)->withTimeout(10),

      DriveTankRawCmd(0.4, 0.4),
      new DelayCommand(4000),
      (new Parallel({
        drive_sys.DriveToPointCmd({24, 118}, vex::reverse, 1)->withTimeout(2),
        new InOrder({new DelayCommand(100), LoadUpCmd(), LiftUpCmd(), IntakeStopCmd()}),
      }))->withTimeout(1.5),
      odom.SetPositionCmd(Pose2d(14, 118, from_degrees(180))),

      drive_sys.TurnToHeadingCmd(-135, 0.6)->withTimeout(1),
      drive_sys.FollowTrajectoryCmd(traj_left_drive_to_blue, skills_config)->withTimeout(10),

      drive_sys.TurnToHeadingCmd(0, 0.6)->withTimeout(0.5),
      drive_sys.DriveToPointCmd({105, 117}, vex::reverse, 1)->withTimeout(2),
      HoodOpenCmd(),
      DriveTankRawCmd(-0.3, -0.3),
      new DelayCommand(1000),
      ScoreUpperCmd(),
      odom.SetPositionCmd(Pose2d(105, 118, from_degrees(0))),
      new DelayCommand(500),

      drive_sys.DriveToPointCmd({124, 117}, vex::forward, 1)->withTimeout(1),
      drive_sys.TurnToHeadingCmd(0, 0.6)->withTimeout(0.5),
      LoadDownCmd(),
      IntakeCmd(),
      new DelayCommand(500),

      DriveTankRawCmd(0.40, 0.40),
      new DelayCommand(4000),

      (new Parallel({
        drive_sys.DriveToPointCmd({102, 117}, vex::reverse, 1)->withTimeout(1.5),
        new InOrder({new DelayCommand(100), LoadUpCmd(), LiftUpCmd(), IntakeStopCmd(), HoodOpenCmd()}),
      }))->withTimeout(1.5),
      drive_sys.TurnToHeadingCmd(0, 0.5)->withTimeout(0.7),
      HoodOpenCmd(),
      DriveTankRawCmd(-0.3, -0.3),
      LoadUpCmd(),
      new DelayCommand(250),

      new DelayCommand(750),
      HoodOpenCmd(),
      ScoreUpperSlowCmd(),
      odom.SetPositionCmd(Pose2d(105, 118, from_degrees(0))),
      new DelayCommand(1000),

      DriveTankRawCmd(0.3, 0.3),
      new DelayCommand(500),
      DriveTankRawCmd(0, 0),
      drive_sys.DriveToPointCmd({118, 118}, vex::forward, 0.5)->withTimeout(1),
      drive_sys.TurnToHeadingCmd(-90, 0.6)->withTimeout(0.7),

      drive_sys.FollowTrajectoryCmd(traj_right_to_left_far_side, skills_config)->withTimeout(10),

      drive_sys.TurnToHeadingCmd(0, 0.6)->withTimeout(0.7),
      LoadDownCmd(),
      IntakeCmd(),
      new DelayCommand(500),
      DriveTankRawCmd(0.40, 0.40),
      new DelayCommand(3000),
      odom.SetPositionCmd(Pose2d(132, 24, from_degrees(0))),
      new DelayCommand(1000),

      new Parallel({
        drive_sys.FollowTrajectoryCmd(traj_far_side_loader_to_middle, skills_config)->withTimeout(10),
        new InOrder({new DelayCommand(100), LoadUpCmd(), WingDownCmd(), LiftDownCmd(), IntakeStopCmd()}),
      }),

      new DelayCommand(250),
      ScoreLowerCmd(),
      new DelayCommand(500),

      DriveTankRawCmd(0.3, 0.3),
      LiftUpCmd(),
      new DelayCommand(100),

      drive_sys.FollowTrajectoryCmd(traj_return_to_far_left, skills_config)->withTimeout(10),
      drive_sys.TurnToHeadingCmd(-45, 0.5)->withTimeout(1),

      drive_sys.FollowTrajectoryCmd(traj_left_blue_to_red, skills_config)->withTimeout(10),

      DriveTankRawCmd(-0.45, -0.45),
    };
    cc.run();
}

void skills_slow() {
    TankTrajectoryFollowerConfig skills_config = trajectory_follower_config;
    skills_config.q_tolerances = {0.5, 0.5, 1, 1, 1};

    Trajectory traj_left_to_loader = left_to_loader();
    Trajectory traj_left_drive_to_blue = left_drive_to_blue();
    Trajectory traj_left_blue_to_red = left_blue_to_red();

    CommandController cc{
      LoadDownCmd(),
      IntakeCmd(),
      drive_sys.FollowTrajectoryCmd(traj_left_to_loader, skills_config)->withTimeout(10),

      DriveTankRawCmd(0.4, 0.4),
      new DelayCommand(4000),
      (new Parallel({
        drive_sys.DriveToPointCmd({24, 118}, vex::reverse, 1)->withTimeout(2),
        new InOrder({new DelayCommand(100), LoadUpCmd(), LiftUpCmd(), IntakeStopCmd()}),
      }))->withTimeout(1.5),
      odom.SetPositionCmd(Pose2d(14, 118, from_degrees(180))),

      drive_sys.TurnToHeadingCmd(-135, 0.6)->withTimeout(1),
      drive_sys.FollowTrajectoryCmd(traj_left_drive_to_blue, skills_config)->withTimeout(10),

      drive_sys.TurnToHeadingCmd(0, 0.6)->withTimeout(3),
      drive_sys.DriveToPointCmd({105, 118}, vex::reverse, 1)->withTimeout(2),
      HoodOpenCmd(),
      DriveTankRawCmd(-0.3, -0.3),
      new DelayCommand(1000),
      ScoreUpperCmd(),
      odom.SetPositionCmd(Pose2d(105, 118, from_degrees(0))),
      new DelayCommand(500),

      drive_sys.DriveToPointCmd({124, 118.25}, vex::forward, 1)->withTimeout(1),
      drive_sys.TurnToHeadingCmd(0, 0.6)->withTimeout(2),
      LoadDownCmd(),
      IntakeCmd(),
      new DelayCommand(500),

      DriveTankRawCmd(0.40, 0.40),
      new DelayCommand(4000),

      (new Parallel({
        drive_sys.DriveToPointCmd({102, 117.5}, vex::reverse, 1)->withTimeout(3),
        new InOrder({new DelayCommand(100), LoadUpCmd(), LiftUpCmd(), IntakeStopCmd(), HoodOpenCmd()}),
      }))->withTimeout(1.5),
      drive_sys.TurnToHeadingCmd(0, 0.5)->withTimeout(2),
      HoodOpenCmd(),
      DriveTankRawCmd(-0.3, -0.3),
      LoadUpCmd(),
      new DelayCommand(250),

      new DelayCommand(750),
      HoodOpenCmd(),
      ScoreUpperSlowCmd(),
      odom.SetPositionCmd(Pose2d(105, 118, from_degrees(0))),
      new DelayCommand(1000),

      DriveTankRawCmd(0.3, 0.3),
      new DelayCommand(500),
      DriveTankRawCmd(0, 0),
      drive_sys.DriveToPointCmd({118, 118}, vex::forward, 0.5)->withTimeout(3),
      drive_sys.TurnToHeadingCmd(-45, 0.6)->withTimeout(1.5),

      drive_sys.FollowTrajectoryCmd(traj_left_blue_to_red, skills_config)->withTimeout(10),

      DriveTankRawCmd(-0.55, -0.55),
    };
    cc.run();
}
