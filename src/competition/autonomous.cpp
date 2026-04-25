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
void red_left_extended();
void red_right();
void red_right_extended();
void awp_right();

void skills();
void skills_longer();
void skills_slow();

void autonomous() {
    // skills();
    // skills_longer();
    // skills_slow();
    // red_left();
    red_left_extended();
    // red_right();
    // red_right_extended();
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






Trajectory left_to_loader() {
  std::vector<HermitePoint> points = {
    {19.000, 87.500, 0.000, 35.000},
    {12.000, 116.500, -60.000, 0.000},
  };

  TrajectoryConfig config(80.000_inps, 80.000_inps2);
  config.set_start_velocity(0.000_inps);
  config.set_end_velocity(22.000_inps);
  config.set_reversed(false);
  config.set_track_width(12.000_in);
  config.add_constraint(CentripetalAccelerationConstraint(100.000_inps2));
  config.add_constraint(TankVoltageConstraint(0.118581_VpInps, 0.016758_VpInps2, 12.000_V, 12.000_in));
  return TrajectoryGenerator::generate_trajectory(points, config);
}

Trajectory left_loader_to_goal() {  
  std::vector<HermitePoint> points = {
    {12.000, 117.500, 30.000, 0.000},
    {32.000, 118.500, 20.000, 0.000},
    {40.000, 118.500, 20.000, 0.000},
  };

  TrajectoryConfig config(90.000_inps, 120.000_inps2);
  config.set_start_velocity(0.000_inps);
  config.set_end_velocity(20.000_inps);
  config.set_reversed(true);
  config.set_track_width(11.8_in);
  config.add_constraint(CentripetalAccelerationConstraint(180.000_inps2));
  config.add_constraint(TankVoltageConstraint(0.175_VpInPs, 0.042_VpInPs2, 12.000_V, 12.000_in));
  Trajectory trajectory = TrajectoryGenerator::generate_trajectory(points, config);

  return trajectory;
}

Trajectory left_loader_to_spit() {  
  std::vector<HermitePoint> points = {
    {12.000, 116.500, 30.000, 0.000},
    {32.000, 108.000, 0.000, -30.000},
  };

  TrajectoryConfig config(60.000_inps, 60.000_inps2);
  config.set_start_velocity(0.000_inps);
  config.set_end_velocity(0.000_inps);
  config.set_reversed(true);
  config.set_track_width(11.8_in);
  config.add_constraint(CentripetalAccelerationConstraint(180.000_inps2));
  config.add_constraint(TankVoltageConstraint(0.175_VpInPs, 0.042_VpInPs2, 12.000_V, 12.000_in));
  Trajectory trajectory = TrajectoryGenerator::generate_trajectory(points, config);

  return trajectory;
}

Trajectory left_spit_to_loader() {  
  std::vector<HermitePoint> points = {
    {32.000, 108.000, 0.000, 30.000},
    {12.000, 117.500, -30.000, 0.000},
  };

  TrajectoryConfig config(60.000_inps, 60.000_inps2);
  config.set_start_velocity(0.000_inps);
  config.set_end_velocity(20.000_inps);
  config.set_reversed(false);
  config.set_track_width(11.8_in);
  config.add_constraint(CentripetalAccelerationConstraint(180.000_inps2));
  config.add_constraint(TankVoltageConstraint(0.175_VpInPs, 0.042_VpInPs2, 12.000_V, 12.000_in));
  Trajectory trajectory = TrajectoryGenerator::generate_trajectory(points, config);

  return trajectory;
}

Trajectory left_goal_to_loader() {  
  std::vector<HermitePoint> points = {
    {40.000, 118.500, -20.000, 0.000},
    {32.000, 118.500, -20.000, 0.000},
    {12.000, 117.500, -30.000, 0.000},
  };

  TrajectoryConfig config(40.000_inps, 60.000_inps2);
  config.set_start_velocity(0.000_inps);
  config.set_end_velocity(20.000_inps);
  config.set_reversed(false);
  config.set_track_width(11.8_in);
  config.add_constraint(CentripetalAccelerationConstraint(180.000_inps2));
  config.add_constraint(TankVoltageConstraint(0.175_VpInPs, 0.042_VpInPs2, 12.000_V, 12.000_in));
  Trajectory trajectory = TrajectoryGenerator::generate_trajectory(points, config);

  return trajectory;
}

Trajectory left_reverse_setup_wing() {
  std::vector<HermitePoint> points = {
    {42.000, 119.000, -10.000, 0.000},
    {20.000, 115.000, -10.000, -10.000},
  };

  TrajectoryConfig config(80.000_inps, 60.000_inps2);
  config.set_start_velocity(0.000_inps);
  config.set_end_velocity(0.000_inps);
  config.set_reversed(false);
  config.set_track_width(12.000_in);
  config.add_constraint(CentripetalAccelerationConstraint(120.000_inps2));
  config.add_constraint(TankVoltageConstraint(0.118581_VpInps, 0.016758_VpInps2, 12.000_V, 12.000_in));
  return TrajectoryGenerator::generate_trajectory(points, config);
}

Trajectory left_wing() {
  std::vector<HermitePoint> points = {
    {20.000, 115.000, 28.000, 28.000},
    {39.000, 129.000, 20.000, 0.000},
    {62.000, 129.000, 30.000, 0.000},
  };

  TrajectoryConfig config(80.000_inps, 60.000_inps2);
  config.set_start_velocity(0.000_inps);
  config.set_end_velocity(0.000_inps);
  config.set_reversed(true);
  config.set_track_width(12.000_in);
  config.add_constraint(CentripetalAccelerationConstraint(120.000_inps2));
  config.add_constraint(TankVoltageConstraint(0.118581_VpInps, 0.016758_VpInps2, 12.000_V, 12.000_in));
  return TrajectoryGenerator::generate_trajectory(points, config);
}








Trajectory right_to_loader() {
  std::vector<HermitePoint> points = {
    {19.000, 55.000, 0.000, -35.000},
    {12, 26, -60, 0.000},
  };

  TrajectoryConfig config(80.000_inps, 80.000_inps2);
  config.set_start_velocity(0.000_inps);
  config.set_end_velocity(22.000_inps);
  config.set_reversed(false);
  config.set_track_width(12.000_in);
  config.add_constraint(CentripetalAccelerationConstraint(100.000_inps2));
  config.add_constraint(TankVoltageConstraint(0.118581_VpInps, 0.016758_VpInps2, 12.000_V, 12.000_in));
  return TrajectoryGenerator::generate_trajectory(points, config);
}

Trajectory right_loader_to_goal() {  
  std::vector<HermitePoint> points = {
    {12.000, 25.000, 30.000, 0.000},
    {32.000, 24.000, 20.000, 0.000},
    {40.000, 24.000, 20.000, 0.000},
  };

  TrajectoryConfig config(90.000_inps, 120.000_inps2);
  config.set_start_velocity(0.000_inps);
  config.set_end_velocity(20.000_inps);
  config.set_reversed(true);
  config.set_track_width(11.8_in);
  config.add_constraint(CentripetalAccelerationConstraint(180.000_inps2));
  config.add_constraint(TankVoltageConstraint(0.175_VpInPs, 0.042_VpInPs2, 12.000_V, 12.000_in));
  Trajectory trajectory = TrajectoryGenerator::generate_trajectory(points, config);

  return trajectory;
}

Trajectory right_loader_to_spit() {  
  std::vector<HermitePoint> points = {
    {12.000, 26.000, 30.000, 0.000},
    {32.000, 34.500, 0.000, 30.000},
  };

  TrajectoryConfig config(60.000_inps, 60.000_inps2);
  config.set_start_velocity(0.000_inps);
  config.set_end_velocity(0.000_inps);
  config.set_reversed(true);
  config.set_track_width(11.8_in);
  config.add_constraint(CentripetalAccelerationConstraint(180.000_inps2));
  config.add_constraint(TankVoltageConstraint(0.175_VpInPs, 0.042_VpInPs2, 12.000_V, 12.000_in));
  Trajectory trajectory = TrajectoryGenerator::generate_trajectory(points, config);

  return trajectory;
}

Trajectory right_spit_to_loader() {  
  std::vector<HermitePoint> points = {
    {32.000, 34.500, 0.000, -30.000},
    {12.000, 25.000, -30.000, 0.000},
  };

  TrajectoryConfig config(60.000_inps, 60.000_inps2);
  config.set_start_velocity(0.000_inps);
  config.set_end_velocity(20.000_inps);
  config.set_reversed(false);
  config.set_track_width(11.8_in);
  config.add_constraint(CentripetalAccelerationConstraint(180.000_inps2));
  config.add_constraint(TankVoltageConstraint(0.175_VpInPs, 0.042_VpInPs2, 12.000_V, 12.000_in));
  Trajectory trajectory = TrajectoryGenerator::generate_trajectory(points, config);

  return trajectory;
}

Trajectory right_goal_to_loader() {  
  std::vector<HermitePoint> points = {
    {40.000, 23.500, -20.000, 0.000},
    {32.000, 23.500, -20.000, 0.000},
    {12.000, 25.000, -30.000, 0.000},
  };

  TrajectoryConfig config(40.000_inps, 60.000_inps2);
  config.set_start_velocity(0.000_inps);
  config.set_end_velocity(20.000_inps);
  config.set_reversed(false);
  config.set_track_width(11.8_in);
  config.add_constraint(CentripetalAccelerationConstraint(180.000_inps2));
  config.add_constraint(TankVoltageConstraint(0.175_VpInPs, 0.042_VpInPs2, 12.000_V, 12.000_in));
  Trajectory trajectory = TrajectoryGenerator::generate_trajectory(points, config);

  return trajectory;
}

Trajectory right_reverse_setup_wing() {
  std::vector<HermitePoint> points = {
    {42.000, 24.000, -10.000, 0.000},
    {20.000, 20.000, -10.000, -10.000},
  };

  TrajectoryConfig config(80.000_inps, 60.000_inps2);
  config.set_start_velocity(0.000_inps);
  config.set_end_velocity(0.000_inps);
  config.set_reversed(false);
  config.set_track_width(12.000_in);
  config.add_constraint(CentripetalAccelerationConstraint(120.000_inps2));
  config.add_constraint(TankVoltageConstraint(0.118581_VpInps, 0.016758_VpInps2, 12.000_V, 12.000_in));
  return TrajectoryGenerator::generate_trajectory(points, config);
}

Trajectory right_wing() {
  std::vector<HermitePoint> points = {
    {20.000, 20.000, 28.000, 28.000},
    {39.000, 34.000, 20.000, 0.000},
    {62.000, 34.000, 30.000, 0.000},
  };

  TrajectoryConfig config(80.000_inps, 60.000_inps2);
  config.set_start_velocity(0.000_inps);
  config.set_end_velocity(0.000_inps);
  config.set_reversed(true);
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

void awp_right() {}

void red_right() {
    printf("red right\n");

    Trajectory traj_right_to_loader = right_to_loader();
    Trajectory traj_right_wing = right_wing();
    Trajectory traj_right_reverse_setup_wing = right_reverse_setup_wing();
    Trajectory traj_right_loader_to_goal = right_loader_to_goal();

    Trajectory grouped_wing = traj_right_reverse_setup_wing + traj_right_wing;

    CommandController cc{
      new Parallel({
        drive_sys.FollowTrajectoryCmd(traj_right_to_loader, trajectory_follower_config)->withTimeout(10),
        LoadDownCmd(),
        IntakeCmd(),
      }),

      DriveTankRawCmd(0.1, 0.1),
      new DelayCommand(400),
      DriveTankRawCmd(0, 0),
      new DelayCommand(200),

      (new Parallel({
        drive_sys.FollowTrajectoryCmd(traj_right_loader_to_goal, trajectory_follower_config),
        new InOrder({new DelayCommand(100), LoadUpCmd(), LiftUpCmd(), HoodOpenCmd(), new DelayCommand(700), new Async(ScoreUpperCmd()), WingDownCmd(), IntakeStopCmd()}),
      }))->withTimeout(3.5),
      DriveTankRawCmd(0, 0),

      new Parallel({
        drive_sys.FollowTrajectoryCmd(grouped_wing, trajectory_follower_config)->withTimeout(10),
        WingDownCmd(),
      }),

      BrakeDriveCmd(vex::brakeType::hold),
    };
    cc.run();
}

void red_left() {
    printf("red left\n");

    Trajectory traj_left_to_loader = left_to_loader();
    Trajectory traj_left_wing = left_wing();
    Trajectory traj_left_reverse_setup_wing = left_reverse_setup_wing();
    Trajectory traj_left_loader_to_goal = left_loader_to_goal();

    Trajectory grouped_wing = traj_left_reverse_setup_wing + traj_left_wing;

    CommandController cc{
      new Parallel({
        drive_sys.FollowTrajectoryCmd(traj_left_to_loader, trajectory_follower_config)->withTimeout(10),
        LoadDownCmd(),
        IntakeCmd(),
      }),

      DriveTankRawCmd(0.1, 0.1),
      new DelayCommand(400),
      DriveTankRawCmd(0, 0),
      new DelayCommand(200),
      (new Parallel({
        drive_sys.FollowTrajectoryCmd(traj_left_loader_to_goal, trajectory_follower_config),
        new InOrder({new DelayCommand(100), LoadUpCmd(), LiftUpCmd(), HoodOpenCmd(), new DelayCommand(700), new Async(ScoreUpperCmd()), WingDownCmd(), IntakeStopCmd()}),
      }))->withTimeout(3.5),
      DriveTankRawCmd(0, 0),

      new Parallel({
        drive_sys.FollowTrajectoryCmd(grouped_wing, trajectory_follower_config)->withTimeout(10),
        WingDownCmd(),
      }),

      BrakeDriveCmd(vex::brakeType::hold),
    };
    cc.run();
}

void red_left_extended() {
    printf("red left\n");

    Trajectory traj_left_to_loader = left_to_loader();
    Trajectory traj_left_loader_to_goal = left_loader_to_goal();
    Trajectory traj_left_goal_to_loader = left_goal_to_loader();
    Trajectory traj_left_loader_to_spit = left_loader_to_spit();
    Trajectory traj_left_spit_to_loader = left_spit_to_loader();
    Trajectory traj_left_wing = left_wing();
    Trajectory traj_left_reverse_setup_wing = left_reverse_setup_wing();

    Trajectory grouped_spit = traj_left_loader_to_spit + traj_left_spit_to_loader;
    Trajectory grouped_wing = traj_left_reverse_setup_wing + traj_left_wing;

    CommandController cc{
      new Parallel({
        drive_sys.FollowTrajectoryCmd(traj_left_to_loader, trajectory_follower_config)->withTimeout(10),
        LoadDownCmd(),
        IntakeCmd(),
      }),

      DriveTankRawCmd(0.3, 0.3),
      new DelayCommand(400),
      DriveTankRawCmd(0, 0),
      (new Parallel({
        new InOrder({drive_sys.FollowTrajectoryCmd(traj_left_loader_to_goal, trajectory_follower_config), DriveTankRawCmd(-0.5, -0.5)}),
        new InOrder({new DelayCommand(100), LiftUpCmd(), HoodOpenCmd(), new DelayCommand(1000), new Async(new InOrder({ScoreUpperCmd(), IntakeCmd()})), new DelayCommand(100), WingDownCmd()}),
      }))->withTimeout(3.5),
      new DelayCommand(600),
      DriveTankRawCmd(0, 0),
      new DelayCommand(200),

      new Parallel({
        new InOrder({drive_sys.FollowTrajectoryCmd(traj_left_goal_to_loader, trajectory_follower_config), DriveTankRawCmd(0.3, 0.3)}),
        LoadDownCmd(),
        IntakeCmd(),
      }),
      IntakeCmd(),
      DriveTankRawCmd(0.3, 0.3),
      new DelayCommand(1000),
      DriveTankRawCmd(0, 0),
      IntakeCmd(),


      new Parallel({
          new InOrder({drive_sys.FollowTrajectoryCmd(grouped_spit, trajectory_follower_config), DriveTankRawCmd(0.3, 0.3)}),
          new InOrder({new DelayCommand(900), ScoreUpperSlowCmd(), IntakeCmd()}),
      }),
      new DelayCommand(1000),
      DriveTankRawCmd(0, 0),
      new DelayCommand(100),


      (new Parallel({
        new InOrder({drive_sys.FollowTrajectoryCmd(traj_left_loader_to_goal, trajectory_follower_config), DriveTankRawCmd(-0.5, -0.5)}),
        new InOrder({new DelayCommand(100), LoadUpCmd(), LiftUpCmd(), HoodOpenCmd(), new DelayCommand(1000), new Async(ScoreUpperCmd()), new DelayCommand(100), WingDownCmd()}),
      }))->withTimeout(3.5),
      new DelayCommand(600),
      DriveTankRawCmd(0, 0),
      new DelayCommand(200),

      new Parallel({
        WingDownCmd(),
        drive_sys.FollowTrajectoryCmd(grouped_wing, trajectory_follower_config)->withTimeout(10),
      }),

      BrakeDriveCmd(vex::brakeType::hold),
    };
    cc.run();
}

void red_right_extended() {
    printf("red right\n");

    Trajectory traj_right_to_loader = right_to_loader();
    Trajectory traj_right_loader_to_goal = right_loader_to_goal();
    Trajectory traj_right_goal_to_loader = right_goal_to_loader();
    Trajectory traj_right_loader_to_spit = right_loader_to_spit();
    Trajectory traj_right_spit_to_loader = right_spit_to_loader();
    Trajectory traj_right_wing = right_wing();
    Trajectory traj_right_reverse_setup_wing = right_reverse_setup_wing();

    Trajectory grouped_spit = traj_right_loader_to_spit + traj_right_spit_to_loader;
    Trajectory grouped_wing = traj_right_reverse_setup_wing + traj_right_wing;

    CommandController cc{
      new Parallel({
        drive_sys.FollowTrajectoryCmd(traj_right_to_loader, trajectory_follower_config)->withTimeout(10),
        LoadDownCmd(),
        IntakeCmd(),
      }),

      DriveTankRawCmd(0.3, 0.3),
      new DelayCommand(400),
      DriveTankRawCmd(0, 0),
      (new Parallel({
        new InOrder({drive_sys.FollowTrajectoryCmd(traj_right_loader_to_goal, trajectory_follower_config), DriveTankRawCmd(-0.5, -0.5)}),
        new InOrder({new DelayCommand(100), LiftUpCmd(), HoodOpenCmd(), new DelayCommand(1000), new Async(new InOrder({ScoreUpperCmd(), IntakeCmd()})), new DelayCommand(100), WingDownCmd()}),
      }))->withTimeout(3.5),
      new DelayCommand(600),
      DriveTankRawCmd(0, 0),
      new DelayCommand(200),

      new Parallel({
        new InOrder({drive_sys.FollowTrajectoryCmd(traj_right_goal_to_loader, trajectory_follower_config), DriveTankRawCmd(0.3, 0.3)}),
        LoadDownCmd(),
        IntakeCmd(),
      }),
      IntakeCmd(),
      DriveTankRawCmd(0.3, 0.3),
      new DelayCommand(1000),
      DriveTankRawCmd(0, 0),
      IntakeCmd(),

      new Parallel({
          new InOrder({drive_sys.FollowTrajectoryCmd(grouped_spit, trajectory_follower_config), DriveTankRawCmd(0.3, 0.3)}),
          new InOrder({new DelayCommand(900), ScoreUpperSlowCmd(), IntakeCmd()}),
      }),
      new DelayCommand(1000),
      DriveTankRawCmd(0, 0),
      new DelayCommand(100),


      (new Parallel({
        new InOrder({drive_sys.FollowTrajectoryCmd(traj_right_loader_to_goal, trajectory_follower_config), DriveTankRawCmd(-0.5, -0.5)}),
        new InOrder({new DelayCommand(100), LoadUpCmd(), LiftUpCmd(), HoodOpenCmd(), new DelayCommand(1000), new Async(ScoreUpperCmd()), new DelayCommand(100), WingDownCmd()}),
      }))->withTimeout(3.5),
      new DelayCommand(600),
      DriveTankRawCmd(0, 0),
      new DelayCommand(200),

      new Parallel({
        WingDownCmd(),
        drive_sys.FollowTrajectoryCmd(grouped_wing, trajectory_follower_config)->withTimeout(10),
      }),

      BrakeDriveCmd(vex::brakeType::hold),
    };
    cc.run();
}
