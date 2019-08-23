import sys_max30001
import interrupt
import ucollections


class MAX30001:
    """
    The MAX30001 class provides a stram interface to the MAX30001
    ECG and BIO-Z sensor.

    .. code-block:: python

        import max30001
        m = max30001.MAX30001()
        m.read()
        m.close()
    """

    def __init__(
        self,
        usb=False,
        bias=True,
        sample_rate=128,
        callback=None,
        sample_buffer_len=256,
    ):
        """
        Initializes the MAX30001 (if it is not already running).

        :param usb: True if all ECG leads should use the USB-C connector
        :param bias: True if the internal bias of the ECG should be used. Mandatory if the card10 is not attached to the body and the USB-C connector is used.
        :param sample_rate: Selected sample rate in Hz. Supported values: 128 and 256.
        :param callback: If not None: A callback which is called with the data when ever new data is available
        :param sample_buffer: Length of the internal buffer (in samples)
        """

        self.sample_rate = sample_rate
        self.callback = callback
        self.sample_buffer_len = sample_buffer_len
        self.interrupt_id = interrupt.MAX30001_ECG
        self.usb = usb
        self.bias = bias
        self._callback = callback
        self.enable_sensor()

    def enable_sensor(self):
        """
        Enables the sensor. Automatically called by __init__.
        """
        interrupt.disable_callback(self.interrupt_id)
        interrupt.set_callback(self.interrupt_id, self._interrupt)
        self.stream_id = sys_max30001.enable_sensor(
            self.usb, self.bias, self.sample_rate, self.sample_buffer_len
        )

        if self.stream_id < 0:
            raise ValueError("Enable sensor returned %i", self.stream_id)

        self.active = True

        if self._callback:
            interrupt.enable_callback(self.interrupt_id)

    def __enter__(self):
        return self

    def __exit__(self, _et, _ev, _t):
        self.close()

    def close(self):
        """
        Close the currently open connection to the sensor.
        """

        if self.active:
            self.active = False
            ret = sys_max30001.disable_sensor()

            if ret < 0:
                raise ValueError("Disable sensor returned %i", ret)

            interrupt.disable_callback(self.interrupt_id)
            interrupt.set_callback(self.interrupt_id, None)

    def read(self):
        """
        Read as many samples (signed integer) as currently available.
        """
        if self.active:
            return sys_max30001.read_sensor(self.stream_id)
        return []

    def _interrupt(self, _):
        if self.active:
            data = self.read()
            if self._callback:
                self._callback(data)
