import os


def main():
    # Try loading analog clock
    default_app = "apps/analog_clock/__init__.py"
    try:
        with open(default_app, "r"):
            pass

        print("main.py: Loading " + default_app)
        os.exec(default_app)
    finally:
        os.exit(1)


if __name__ == "__main__":
    main()
