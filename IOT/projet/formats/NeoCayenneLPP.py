# Copyright (C) 2023  Malik Irain
# This file is part of sense-hat-iot-project.
#
# sense-hat-iot-project is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# sense-hat-iot-project is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with sense-hat-iot-project.  If not, see <http://www.gnu.org/licenses/>.

import json
from enum import Enum
from struct import pack, unpack
from typing import List, TypedDict, Union


class NeoCayenneLPP:
	class RawTupleException(Exception):
		def __init__(self, a_tuple: 'NeoCayenneLPP.Tuple'):
			self.message = f'NeoCayenneLPP Trame already contains tuples; \'{{"type_": {a_tuple["type_"].value}, "channel": {a_tuple["channel"]}, "value": "{a_tuple["value"].hex()}"}}\' can\'t be added.'
			super().__init__(self.message)
	class PayloadOverflowException(Exception):
		def __init__(self, payload_length : int, tuple_len : int):
			self.message = "Can't add {} bytes of data, payload is already {} bytes. ({} > 255)".format(tuple_len, payload_length, payload_length + tuple_len)
			super().__init__(self.message)

	class Version(Enum):
		NEOCAMPUS_LORA_V1 = 42
		ECONECT_LORA_V1   = 43

	class Type(Enum):
		LPP_RAW             = 0
		LPP_RAW_ANALOG      = 1
		LPP_RAW_ANALOG_EXT  = 2
		LPP_RAW_DIGITAL     = 3
		LPP_LUMINOSITY      = 4
		LPP_PRESENCE        = 5
		LPP_FREQUENCY       = 6
		LPP_TEMPERATURE     = 7
		LPP_HUMIDITY        = 8
		LPP_CO2             = 9
		LPP_RAW_AIR_QUALITY = 10 
		LPP_RAW_GPS         = 11
		LPP_ENERGY          = 12
		LPP_ENERGY_SOLAR    = 13
		LPP_ENERGY_WATT     = 14
		LPP_ENERGY_VOLT     = 15
		LPP_ENERGY_AMP      = 16
		LPP_ENERGY_PHASE    = 17
		LPP_UV              = 20
		LPP_UV_ENERGY       = 21
		LPP_WEIGHT          = 22
		LPP_PRESSURE        = 23

		@property
		def size(self):
			size = {
				NeoCayenneLPP.Type.LPP_RAW             : 255,
				NeoCayenneLPP.Type.LPP_RAW_ANALOG      : 2,
				NeoCayenneLPP.Type.LPP_RAW_ANALOG_EXT  : 4,
				NeoCayenneLPP.Type.LPP_RAW_DIGITAL     : 1,
				NeoCayenneLPP.Type.LPP_LUMINOSITY      : 2,
				NeoCayenneLPP.Type.LPP_PRESENCE        : 1,
				NeoCayenneLPP.Type.LPP_FREQUENCY       : 2,
				NeoCayenneLPP.Type.LPP_TEMPERATURE     : 1,
				NeoCayenneLPP.Type.LPP_HUMIDITY        : 1,
				NeoCayenneLPP.Type.LPP_CO2             : 2,
				NeoCayenneLPP.Type.LPP_RAW_AIR_QUALITY : 2,
				NeoCayenneLPP.Type.LPP_RAW_GPS         : 9,
				NeoCayenneLPP.Type.LPP_ENERGY          : 2,
				NeoCayenneLPP.Type.LPP_ENERGY_SOLAR    : 2,
				NeoCayenneLPP.Type.LPP_ENERGY_WATT     : 2,
				NeoCayenneLPP.Type.LPP_ENERGY_VOLT     : 1,
				NeoCayenneLPP.Type.LPP_ENERGY_AMP      : 1,
				NeoCayenneLPP.Type.LPP_ENERGY_PHASE    : 2,


				NeoCayenneLPP.Type.LPP_UV              : 1,
				NeoCayenneLPP.Type.LPP_UV_ENERGY       : 1,
				NeoCayenneLPP.Type.LPP_WEIGHT          : 2,
				NeoCayenneLPP.Type.LPP_PRESSURE        : 1,				
			}
			return size[self]


	class Tuple(TypedDict):
		type_ : 'NeoCayenneLPP.Type'
		channel : int
		value : bytes



	__slots__ = ('_format_version', '_payload_length', '_tuples', '__contains_raw_data')

	def __init__(self, version : 'NeoCayenneLPP.Version' = Version.ECONECT_LORA_V1):
		self._format_version = version
		self._payload_length  = 2
		self._tuples : List[NeoCayenneLPP.Tuple] = []
		
		self.__contains_raw_data  = False

	@property
	def length(self) -> int:
		return self._payload_length

	@property
	def encoded_tuples(self) -> List['NeoCayenneLPP.Tuple']:
		return [*self._tuples]

	def add_raw(self, channel : int, value : bytes) -> None:
		a_tuple : 'NeoCayenneLPP.Tuple'  = {'type_' : NeoCayenneLPP.Type.LPP_RAW, 'channel' : channel, 'value' : value[0: NeoCayenneLPP.Type.LPP_RAW.size]}
		if self._tuples:
			raise NeoCayenneLPP.RawTupleException(a_tuple)

		self._tuples.append(a_tuple)
		self._payload_length += 2 + len(a_tuple['value'])
		
		self.__contains_raw_data = True


	def _add_generic(self, type_ : 'NeoCayenneLPP.Type', channel : int, value : bytes):
		length = 2 + type_.size
		a_tuple : 'NeoCayenneLPP.Tuple' = {'type_' : type_, 'channel' : channel, 'value' : value[0: type_.size]}
		if self._payload_length + length > 255:
			raise NeoCayenneLPP.PayloadOverflowException(self._payload_length, length)
		elif self.__contains_raw_data:
			raise NeoCayenneLPP.RawTupleException(a_tuple)

		self._tuples.append(a_tuple)
		self._payload_length += length

	def add_raw_analog(self, channel : int, value : bytes) -> None:
		self._add_generic(NeoCayenneLPP.Type.LPP_RAW_ANALOG, channel, value)

	def add_raw_analog_ext(self, channel : int, value : bytes) -> None:
		self._add_generic(NeoCayenneLPP.Type.LPP_RAW_ANALOG_EXT, channel, value)

	def add_raw_digital_ext(self, channel : int, value : bytes) -> None:
		self._add_generic(NeoCayenneLPP.Type.LPP_RAW_DIGITAL, channel, value)

	def add_luminosity(self, channel : int, value : Union[int,float]) -> None:
		if value < 0 or value > 65535:
			raise ValueError('Luminosity must be in the range [0 ; 65535] lux. Provided value {} is outside this range.'.format(value))

		luminosity = pack('!H', round(value))
		self._add_generic(NeoCayenneLPP.Type.LPP_LUMINOSITY, channel, luminosity)

	def add_presence(self, channel : int, value : Union[int, float, bool]) -> None:
		if value < 0 or value > 1:
			raise ValueError('Presence must be in the range [0 ; 1]. Provided value {} is outside this range.'.format(value))

		presence = pack('B', round(value))
		self._add_generic(NeoCayenneLPP.Type.LPP_PRESENCE, channel, presence)

	def add_frequency(self, channel : int, value : Union[int,float]) -> None:
		if value < 0 or value > 65535:
			raise ValueError('Frequency must be in the range [0 ; 65535]. Provided value {} is outside this range.'.format(value))

		frequency = pack('!H', round(value))
		self._add_generic(NeoCayenneLPP.Type.LPP_FREQUENCY, channel, frequency)

	def add_temperature(self, channel : int, value : Union[int,float]) -> None:
		if value < -11.75 or value > 51.75:
			raise ValueError('Temperature must be in the range [-11.75 ; 51.75]°C. Provided value {} is outside this range.'.format(value))

		temperature = pack('b', round((value - 20)/0.25))
		self._add_generic(NeoCayenneLPP.Type.LPP_TEMPERATURE, channel, temperature)


	def add_humidity(self, channel : int, value : Union[int,float]) -> None:
		if value < 0 or value > 100:
			raise ValueError('Percentage of relative humidity must be in the range [0 ; 100]%. Provided value {} is outside this range.'.format(value))

		humidity = pack('B', round(value/0.5))
		self._add_generic(NeoCayenneLPP.Type.LPP_HUMIDITY, channel, humidity)

	def add_co2(self, channel : int, value : Union[int,float]) -> None:
		if value < 0 or value > 65535:
			raise ValueError('CO2 level must be in the range [0 ; 65535] ppm. Provided value {} is outside this range.'.format(value))
		co2 = pack('!H', round(value))
		self._add_generic(NeoCayenneLPP.Type.LPP_CO2, channel, co2)

	def add_raw_air_quality(self, channel : int, value : bytes) -> None:
		self._add_generic(NeoCayenneLPP.Type.LPP_RAW_AIR_QUALITY, channel, value)

	def add_raw_gps(self, channel : int, value : bytes) -> None:
		self._add_generic(NeoCayenneLPP.Type.LPP_RAW_GPS, channel, value)


	def add_energy(self, channel : int, value : Union[int,float]) -> None:
		if value < 0 or value > 65535:
			raise ValueError('Energy must be in the range [0 ; 65535] Wh. Provided value {} is outside this range.'.format(value))

		energy = pack('!H', round(value))
		self._add_generic(NeoCayenneLPP.Type.LPP_ENERGY, channel, energy)

	def add_energy_solar(self, channel : int, value : Union[int,float]) -> None:
		if value < 0 or value > 65535:
			raise ValueError('Surface power density must be in the range [0 ; 65535] W/m². Provided value {} is outside this range.'.format(value))

		energy_solar = pack('!H', round(value))
		self._add_generic(NeoCayenneLPP.Type.LPP_ENERGY_SOLAR, channel, energy_solar)

	def add_energy_watt(self, channel : int, value : Union[int,float]) -> None:
		if value < 0 or value > 65535:
			raise ValueError('Power must be in the range [0 ; 65535] W. Provided value {} is outside this range.'.format(value))
		
		energy_watt = pack('!H', round(value))
		self._add_generic(NeoCayenneLPP.Type.LPP_ENERGY_WATT, channel, energy_watt)

	def add_energy_volt(self, channel : int, value : Union[int,float]) -> None:
		if value < 217.2 or value > 242.7:
			raise ValueError('Voltage must be in the range [217.2 ; 242.7] V. Provided value {} is outside this range.'.format(value))

		energy_volt = pack('b', round((value - 230)/0.1))
		self._add_generic(NeoCayenneLPP.Type.LPP_ENERGY_VOLT, channel, energy_volt)


	def add_energy_amp(self, channel : int, value : Union[int,float]) -> None:
		if value < 0 or value > 25.5:
			raise ValueError('Amperage must be in the range [0 ; 25.5] A. Provided value {} is outside this range.'.format(value))
		
		energy_amp = pack('b', round(value/0.1))
		self._add_generic(NeoCayenneLPP.Type.LPP_ENERGY_AMP, channel, energy_amp)


	def add_energy_phase(self, channel : int, value : Union[int,float]) -> None:
		if value < 0 or value > 65535:
			raise ValueError('Phase must be in the range [0 ; 65535] W. Provided value {} is outside this range.'.format(value))

		energy_phase = pack('!H', round(value))
		self._add_generic(NeoCayenneLPP.Type.LPP_ENERGY_PHASE, channel, energy_phase)

	def add_uv(self, channel : int, value : Union[int,float]) -> None:
		if value < 0 or value > 2.55:
			raise ValueError('UV power must be in the range [0 ; 25.5] mW/cm2. Provided value {} is outside this range.'.format(value))

		uv_power = pack('B', round(value/1e-2))
		self._add_generic(NeoCayenneLPP.Type.LPP_UV, channel, uv_power)

	def add_uv_energy(self, channel : int, value : Union[int,float]) -> None:
		if value < 0 or value > 2.55:
			raise ValueError('UV energy must be in the range [0 ; 25.5] mJ/cm2. Provided value {} is outside this range.'.format(value))
		
		uv_energy = pack('B', round(value/1e-2))
		self._add_generic(NeoCayenneLPP.Type.LPP_UV_ENERGY, channel, uv_energy)

	def add_weight(self, channel : int, value : Union[int,float]) -> None:
		if value < 0 or value > 65535:
			raise ValueError('Weight must be in the range [0 ; 65535] g. Provided value {} is outside this range.'.format(value))

		weight = pack('!H', round(value))
		self._add_generic(NeoCayenneLPP.Type.LPP_WEIGHT, channel, weight)

	def add_pressure(self, channel : int, value : Union[int,float]) -> None:
		if value < 990 or value > 1245:
			raise ValueError('Pressure must be in the range [990 ; 1245] mBar. Provided value {} is outside this range.'.format(value))

		pressure = pack('B', round(value - 990))
		self._add_generic(NeoCayenneLPP.Type.LPP_PRESSURE, channel, pressure)

	def to_json(self, prettify=False) -> str:
		payload = ",".join(f'{{"type_": {a_tuple["type_"].value}, "channel": {a_tuple["channel"]}, "value": "{NeoCayenneLPP._decode_value(a_tuple["type_"], a_tuple["value"])}"}}' for a_tuple in self._tuples)
		json_str = f'{{"version": {self._format_version.value}, "length": {self._payload_length}, "payload": [{payload}]}}'
		
		if prettify:
			json_str = json.dumps(json.loads(json_str), indent=2)
		return json_str

	def to_bytes(self) -> bytes:
		res_byte_array = pack('BB', self._format_version.value, self._payload_length) 
		for a_tuple in self._tuples:
			res_byte_array += pack('Bb', a_tuple['type_'].value, a_tuple['channel']) + a_tuple['value']
	
		return res_byte_array



	@staticmethod
	def _decode_value(type_ : 'NeoCayenneLPP.Type', byte_array : bytes) -> str:
		if(  type_ is NeoCayenneLPP.Type.LPP_LUMINOSITY 
		or   type_ is NeoCayenneLPP.Type.LPP_FREQUENCY
		or   type_ is NeoCayenneLPP.Type.LPP_CO2
		or   type_ is NeoCayenneLPP.Type.LPP_ENERGY
		or   type_ is NeoCayenneLPP.Type.LPP_ENERGY_SOLAR
		or   type_ is NeoCayenneLPP.Type.LPP_ENERGY_WATT
		or   type_ is NeoCayenneLPP.Type.LPP_ENERGY_PHASE
		or   type_ is NeoCayenneLPP.Type.LPP_WEIGHT):
			return str(unpack('!H', byte_array)[0])
		elif type_ is NeoCayenneLPP.Type.LPP_PRESENCE:
			return str(unpack('B', byte_array)[0])
		elif type_ is NeoCayenneLPP.Type.LPP_TEMPERATURE:
			return str(unpack('b', byte_array)[0] * 0.25 + 20)
		elif type_ is NeoCayenneLPP.Type.LPP_HUMIDITY:
			return str(unpack('B', byte_array)[0] * 0.5 )
		elif type_ is NeoCayenneLPP.Type.LPP_ENERGY_VOLT:
			return str(unpack('b', byte_array)[0] * 0.1 + 230)
		elif type_ is NeoCayenneLPP.Type.LPP_ENERGY_AMP:
			return str(unpack('b', byte_array)[0] * 0.1)
		elif(type_ is NeoCayenneLPP.Type.LPP_UV
		or   type_ is NeoCayenneLPP.Type.LPP_UV_ENERGY):
			return str(unpack('B', byte_array)[0] * 1e-2)
		elif type_ is NeoCayenneLPP.Type.LPP_PRESSURE:
			return str(unpack('B', byte_array)[0] + 990)
		else:
			return byte_array.hex()

	@staticmethod
	def from_bytes(byte_array : bytes) -> 'NeoCayenneLPP':
		neocayennelpp_trame = NeoCayenneLPP.__new__(NeoCayenneLPP)

		neocayennelpp_trame._format_version = NeoCayenneLPP.Version(byte_array[0])
		neocayennelpp_trame._payload_length = byte_array[1]

		neocayennelpp_trame._tuples = []

		i = 2
		while i < len(byte_array):
			a_type_ = NeoCayenneLPP.Type(byte_array[i])
			a_channel = unpack('b', byte_array[i+1:i+2])[0]
			a_value = byte_array[(i+2) : (i+2+a_type_.size)]
			
			neocayennelpp_trame._tuples.append({'type_' : a_type_, 'channel' : a_channel, 'value' : a_value})
			
			i += 2 + a_type_.size

		return neocayennelpp_trame
