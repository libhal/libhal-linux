# libhal-linux

> [!IMPORTANT]
> There is not yet a published package on JFrog and you will need to build the
> package manually using `conan create . --version=latest`. This will be fixed
> soon.
> No cross compiler profiles have been created yet, to use develop using this
> you currently need develop with the same architecture and operating system as
> your build target. A Dockerfile has been provided for arm64 linux
> development. (Raspberry Pis and alike)

[![✅ Checks](https://github.com/libhal/libhal-linux/actions/workflows/ci.yml/badge.svg)](https://github.com/libhal/libhal-linux/actions/workflows/ci.yml)
[![GitHub stars](https://img.shields.io/github/stars/libhal/libhal-linux.svg)](https://github.com/libhal/libhal-linux/stargazers)
[![GitHub forks](https://img.shields.io/github/forks/libhal/libhal-linux.svg)](https://github.com/libhal/libhal-linux/network)
[![GitHub issues](https://img.shields.io/github/issues/libhal/libhal-linux.svg)](https://github.com/libhal/libhal-linux/issues)

User space library for embedded linux

## 📚 Software APIs & Usage

To learn about available kernel APIs, documentation will be be made available soon.

To see some example code on how various APIs are used, see the programs inside the `demos` folder.

## 🧰 Setup

Following the
[🚀 Getting Started](https://libhal.github.io/2.1/getting_started/)
instructions.

## 📡 Installing Profiles

Coming soon, see important notice.

## 🏗️ Building Demos

> [!NOTE]
> ON the device that is to run this code, a computer that is the same OS and
> architecture, or inside the provided docker container.

To build demos, start at the root of the repo and execute the following command:

```bash
conan build demos
```

## 💾 Flashing/Programming

The easiest way to get your compiled program onto hardware like a Raspberry Pi
is via ssh. (Assuming your hardware's linux installation has sshd).

Run the following command:

```bash
scp path/to/your.bin username@hardware_device:new_path/to/app
```

## Contributing

See [`CONTRIBUTING.md`](CONTRIBUTING.md) for details.

> [!NOTE]
> For git History for implementations of output_pin, input_pin, i2c, and serial
> can be seen [here](https://github.com/PhazonicRidley/libhal-linux-alpha)

## License

Apache 2.0; see [`LICENSE`](LICENSE) for details.
