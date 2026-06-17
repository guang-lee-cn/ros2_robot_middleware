# ============================================================================
# Stage 1: builder — full toolchain: gcc, cmake, colcon, lcov, rosdep
# ============================================================================
FROM docker.io/ros:jazzy-ros-base AS builder

ENV DEBIAN_FRONTEND=noninteractive

# Build + test toolchain
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    gdb \
    git \
    lcov \
    python3-colcon-common-extensions \
    python3-pip \
    python3-rosdep \
    && rm -rf /var/lib/apt/lists/*

RUN rosdep init 2>/dev/null; rosdep update

# Create workspace layout and copy source
WORKDIR /ws
COPY . src/ros2_robot_middleware

# Install ROS2 dependencies and build
RUN . /opt/ros/jazzy/setup.sh && \
    rosdep install --from-paths src --ignore-src -r -y && \
    colcon build --packages-select ros2_robot_middleware \
        --cmake-args -DCMAKE_BUILD_TYPE=Release

# Build and run tests
RUN . /opt/ros/jazzy/setup.sh && \
    . /ws/install/setup.sh && \
    cmake -S src/ros2_robot_middleware/test \
        -B build/ros2_robot_middleware_test \
        -DPROD_BUILD_DIR=/ws/build/ros2_robot_middleware \
        -DCMAKE_PREFIX_PATH="/opt/ros/jazzy;/ws/install" && \
    cmake --build build/ros2_robot_middleware_test && \
    cd build/ros2_robot_middleware_test && \
    ctest --output-on-failure

# ============================================================================
# Stage 2: runtime — minimal footprint, production deploy
# ============================================================================
FROM docker.io/ros:jazzy-ros-core AS runtime

RUN apt-get update && apt-get install -y --no-install-recommends \
    ros-jazzy-ros2cli \
    && rm -rf /var/lib/apt/lists/*

COPY --from=builder /ws/install /ws/install

COPY toolkit/scripts/entrypoint.sh /entrypoint.sh
COPY toolkit/scripts/health-check.sh /health-check.sh
RUN chmod +x /entrypoint.sh /health-check.sh

ENTRYPOINT ["/entrypoint.sh"]
CMD ["lidar"]

# ============================================================================
# Stage 3: dev — builder + interactive development tools
# ============================================================================
FROM builder AS dev

RUN apt-get update && apt-get install -y --no-install-recommends \
    bash-completion \
    nano \
    python3-argcomplete \
    ros-jazzy-rqt-graph \
    ros-jazzy-rqt-console \
    && rm -rf /var/lib/apt/lists/*

COPY toolkit/scripts/entrypoint.sh /entrypoint.sh
COPY toolkit/scripts/health-check.sh /health-check.sh
RUN chmod +x /entrypoint.sh /health-check.sh

ENTRYPOINT ["/entrypoint.sh"]
CMD ["lidar"]
