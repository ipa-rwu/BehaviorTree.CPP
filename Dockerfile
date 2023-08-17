# syntax=docker/dockerfile:1
ARG BUILDER_SUFFIX=
ARG BUILDER_PREFIX=
ARG ROS_DISTRO=
FROM public.ecr.aws/docker/library/ros:${ROS_DISTRO}-ros-core as base
FROM ${BUILDER_PREFIX}builder${BUILDER_SUFFIX} as builder

FROM base as pre_build
COPY . /root/ws/src/
COPY --from=builder workspace.bash /builder/workspace.bash
ARG ROSINSTALL_CI_JOB_TOKEN=
ENV ROSINSTALL_CI_JOB_TOKEN $ROSINSTALL_CI_JOB_TOKEN
ARG CI_JOB_TOKEN=
ENV CI_JOB_TOKEN $CI_JOB_TOKEN
RUN apt-get update -qq && \
    /builder/workspace.bash install_from_rosinstall_folder /root/ws
RUN rm -rf /var/lib/apt/lists/*

FROM pre_build as build
ARG CMAKE_ARGS=
ENV CMAKE_ARGS $CMAKE_ARGS
RUN apt-get update -qq && \
    /builder/workspace.bash build_workspace /root/ws && \
    rm -rf /var/lib/apt/lists/*

FROM build as install
RUN apt-get update -qq && \
    /builder/workspace.bash install_workspace /root/ws && \
    rm -rf /var/lib/apt/lists/*

FROM base as deploy
COPY --from=install /root/ws/DEPENDS /root/ws/DEPENDS
COPY --from=install /ros_entrypoint.sh /ros_entrypoint.sh
COPY --from=builder workspace.bash /builder/workspace.bash
RUN apt-get update -qq && \
    /builder/workspace.bash install_depends /root/ws && \
    rm -rf /var/lib/apt/lists/*
COPY --from=install /root/ws/install /root/ws/install
