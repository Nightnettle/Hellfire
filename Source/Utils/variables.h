#pragma once

namespace GLOBAL
{
	extern HWND csgo_hwnd;

	extern bool should_send_packet;
	extern bool is_fakewalking;
	extern int choke_amount;

	extern Vector real_angles;
	extern Vector fake_angles;
}

class Misc
{
public:

private:
	void DoNoRecoil(SDK::CUserCmd * cmd);

	
};

namespace Colors
{
	#define DEFCOLOR_SRC(name, r, g, b) \
	static Color name##(uint8_t a = 255){ return Color(r, g, b, a); }
	class Color {
	public:

		Color() {
			SetColor(0, 0, 0, 255);
		}

		Color(uint8_t r, uint8_t g, uint8_t b) {
			SetColor(r, g, b, 255);
		}

		Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
			SetColor(r, g, b, a);
		}

		void SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) {
			_color[0] = (uint8_t)r;
			_color[1] = (uint8_t)g;
			_color[2] = (uint8_t)b;
			_color[3] = (uint8_t)a;
		}

		void GetColor(uint8_t &r, uint8_t &g, uint8_t &b, uint8_t &a) const {
			r = _color[0];
			g = _color[1];
			b = _color[2];
			a = _color[3];
		}

		uint8_t &operator[](int index) {
			return _color[index];
		}

		const uint8_t &operator[](int index) const {
			return _color[index];
		}

		bool operator == (const Color &rhs) const {
			return (*((int *)this) == *((int *)&rhs));
		}

		bool operator != (const Color &rhs) const {
			return !(operator==(rhs));
		}

		int r() { return _color[0]; }
		int g() { return _color[1]; }
		int b() { return _color[2]; }
		int a() { return _color[3]; }

		int GetR() { return _color[0]; }
		int GetG() { return _color[1]; }
		int GetB() { return _color[2]; }
		int GetA() { return _color[3]; }

		void SetR(uint8_t _i) { _color[0] = _i; }
		void SetG(uint8_t _i) { _color[1] = _i; }
		void SetB(uint8_t _i) { _color[2] = _i; }
		void SetA(uint8_t _i) { _color[3] = _i; }

		Color &operator=(const Color &rhs) {
			*(int*)(&_color[0]) = *(int*)&rhs._color[0];
			return *this;
		}

		Color operator+(const Color &rhs) const {
			int red = _color[0] + rhs._color[0];
			int green = _color[1] + rhs._color[1];
			int blue = _color[2] + rhs._color[2];
			int alpha = _color[3] + rhs._color[3];

			red = red > 255 ? 255 : red;
			green = green > 255 ? 255 : green;
			blue = blue > 255 ? 255 : blue;
			alpha = alpha > 255 ? 255 : alpha;

			return Color(red, green, blue, alpha);
		}

		Color operator-(const Color &rhs) const {
			int red = _color[0] - rhs._color[0];
			int green = _color[1] - rhs._color[1];
			int blue = _color[2] - rhs._color[2];
			int alpha = _color[3] - rhs._color[3];

			red = red < 0 ? 0 : red;
			green = green < 0 ? 0 : green;
			blue = blue < 0 ? 0 : blue;
			alpha = alpha < 0 ? 0 : alpha;
			return Color(red, green, blue, alpha);
		}

		operator const uint8_t*() {
			return (uint8_t*)(&_color[0]);
		}

		static Color FromHSB(float hue, float saturation, float brightness)
		{
			float h = hue == 1.0f ? 0 : hue * 6.0f;
			float f = h - (int)h;
			float p = brightness * (1.0f - saturation);
			float q = brightness * (1.0f - saturation * f);
			float t = brightness * (1.0f - (saturation * (1.0f - f)));

			if (h < 1)
			{
				return Color(
					(unsigned char)(brightness * 255),
					(unsigned char)(t * 255),
					(unsigned char)(p * 255)
				);
			}
			else if (h < 2)
			{
				return Color(
					(unsigned char)(q * 255),
					(unsigned char)(brightness * 255),
					(unsigned char)(p * 255)
				);
			}
			else if (h < 3)
			{
				return Color(
					(unsigned char)(p * 255),
					(unsigned char)(brightness * 255),
					(unsigned char)(t * 255)
				);
			}
			else if (h < 4)
			{
				return Color(
					(unsigned char)(p * 255),
					(unsigned char)(q * 255),
					(unsigned char)(brightness * 255)
				);
			}
			else if (h < 5)
			{
				return Color(
					(unsigned char)(t * 255),
					(unsigned char)(p * 255),
					(unsigned char)(brightness * 255)
				);
			}
			else
			{
				return Color(
					(unsigned char)(brightness * 255),
					(unsigned char)(p * 255),
					(unsigned char)(q * 255)
				);
			}
		}

		DEFCOLOR_SRC(Black, 0, 0, 0);
		DEFCOLOR_SRC(White, 255, 255, 255);
		DEFCOLOR_SRC(Red, 255, 0, 0);
		DEFCOLOR_SRC(Green, 0, 255, 0);
		DEFCOLOR_SRC(Blue, 0, 0, 255);
		DEFCOLOR_SRC(Lime, 0, 255, 0);
		DEFCOLOR_SRC(Yellow, 255, 255, 0);
		DEFCOLOR_SRC(Cyan, 0, 255, 255);
		DEFCOLOR_SRC(Magenta, 255, 0, 255);
		DEFCOLOR_SRC(Silver, 192, 192, 192);
		DEFCOLOR_SRC(Gray, 128, 128, 128);
		DEFCOLOR_SRC(Maroon, 128, 0, 0);
		DEFCOLOR_SRC(Olive, 128, 128, 0);
		DEFCOLOR_SRC(Purple, 128, 0, 128);
		DEFCOLOR_SRC(Teal, 0, 128, 128);
		DEFCOLOR_SRC(Navy, 0, 0, 128);
		DEFCOLOR_SRC(DarkRed, 139, 0, 0);
		DEFCOLOR_SRC(Brown, 165, 42, 42);
		DEFCOLOR_SRC(Firebrick, 178, 34, 34);
		DEFCOLOR_SRC(Crimson, 220, 20, 60);
		DEFCOLOR_SRC(IndianRed, 205, 92, 92);
		DEFCOLOR_SRC(LightCoral, 240, 128, 128);
		DEFCOLOR_SRC(DarkSalmon, 233, 150, 122);
		DEFCOLOR_SRC(Salmon, 250, 128, 114);
		DEFCOLOR_SRC(LightSalmon, 255, 160, 122);
		DEFCOLOR_SRC(OrangeRed, 255, 69, 0);
		DEFCOLOR_SRC(DarkOrange, 255, 140, 0);
		DEFCOLOR_SRC(Orange, 255, 165, 0);
		DEFCOLOR_SRC(Gold, 255, 215, 0);
		DEFCOLOR_SRC(DarkGoldenRod, 184, 134, 11);
		DEFCOLOR_SRC(GoldenRod, 218, 165, 32);
		DEFCOLOR_SRC(YellowGreen, 154, 205, 50);
		DEFCOLOR_SRC(DarkOliveGreen, 85, 107, 47);
		DEFCOLOR_SRC(OliveDrab, 107, 142, 35);
		DEFCOLOR_SRC(LawnGreen, 124, 252, 0);
		DEFCOLOR_SRC(ChartReuse, 127, 255, 0);
		DEFCOLOR_SRC(GreenYellow, 173, 255, 47);
		DEFCOLOR_SRC(DarkGreen, 0, 100, 0);
		DEFCOLOR_SRC(ForestGreen, 34, 139, 34);
		DEFCOLOR_SRC(LimeGreen, 50, 205, 50);
		DEFCOLOR_SRC(DarkCyan, 0, 139, 139);
		DEFCOLOR_SRC(Aqua, 0, 255, 255);
		DEFCOLOR_SRC(LightCyan, 224, 255, 255);
		DEFCOLOR_SRC(DarkTurquoise, 0, 206, 209);
		DEFCOLOR_SRC(Turquoise, 64, 224, 208);
		DEFCOLOR_SRC(MediumTurquoise, 72, 209, 204);
		DEFCOLOR_SRC(PaleTurquoise, 175, 238, 238);
		DEFCOLOR_SRC(Aquamarine, 127, 255, 212);
		DEFCOLOR_SRC(PowderBlue, 176, 224, 230);
		DEFCOLOR_SRC(DodgerBlue, 30, 144, 255);
		DEFCOLOR_SRC(Lightblue, 173, 216, 230);
		DEFCOLOR_SRC(SkyBlue, 135, 206, 235);
		DEFCOLOR_SRC(LightSkyBlue, 135, 206, 250);
		DEFCOLOR_SRC(MidnightBlue, 25, 25, 112);
		DEFCOLOR_SRC(DarkBlue, 0, 0, 139);
		DEFCOLOR_SRC(MediumBlue, 0, 0, 205);
		DEFCOLOR_SRC(RoyalBlue, 65, 105, 225);
		DEFCOLOR_SRC(BlueViolet, 138, 43, 226);
		DEFCOLOR_SRC(Indigo, 75, 0, 130);
		DEFCOLOR_SRC(DarkSlateBlue, 72, 61, 139);
		DEFCOLOR_SRC(SlateBlue, 106, 90, 205);
		DEFCOLOR_SRC(MediumSlateBlue, 123, 104, 238);
		DEFCOLOR_SRC(MediumPurple, 147, 112, 219);
		DEFCOLOR_SRC(Darkmagenta, 139, 0, 139);
		DEFCOLOR_SRC(Darkviolet, 148, 0, 211);
		DEFCOLOR_SRC(DarkGray, 169, 169, 169);
		DEFCOLOR_SRC(LightGray, 211, 211, 211);
		DEFCOLOR_SRC(Gainsboro, 220, 220, 220);
	private:
		uint8_t _color[4];
	};
}

class Color
{
public:
	Color()
	{
		*((int *)this) = 0;
	}
	Color(int r, int g, int b)
	{
		SetColor(r, g, b, 0);
	}
	Color(int r, int g, int b, int a)
	{
		SetColor(r, g, b, a);
	}
	void SetColor(int r, int g, int b, int a = 0)
	{
		_color[0] = (unsigned char)r;
		_color[1] = (unsigned char)g;
		_color[2] = (unsigned char)b;
		_color[3] = (unsigned char)a;
	}

	static Color FromHSB(float hue, float saturation, float brightness)
	{
		float h = hue == 1.0f ? 0 : hue * 4.0f;
		float f = h - (int)h;
		float p = brightness * (1.0f - saturation);
		float q = brightness * (1.0f - saturation * f);
		float t = brightness * (1.0f - (saturation * (1.0f - f)));

		if (h < 1)
		{
			return Color(
				(unsigned char)(brightness * 255),
				(unsigned char)(t * 255),
				(unsigned char)(p * 255)
			);
		}
		else if (h < 2)
		{
			return Color(
				(unsigned char)(q * 1),
				(unsigned char)(brightness * 255),
				(unsigned char)(p * 255)
			);
		}
		else if (h < 3)
		{
			return Color(
				(unsigned char)(p * 255),
				(unsigned char)(brightness * 255),
				(unsigned char)(t * 255)
			);
		}
		else if (h < 4)
		{
			return Color(
				(unsigned char)(p * 255),
				(unsigned char)(q * 255),
				(unsigned char)(brightness * 255)
			);
		}
		else if (h < 5)
		{
			return Color(
				(unsigned char)(t * 255),
				(unsigned char)(p * 255),
				(unsigned char)(brightness * 255)
			);
		}
		else
		{
			return Color(
				(unsigned char)(brightness * 255),
				(unsigned char)(p * 255),
				(unsigned char)(q * 255)
			);
		}
	}

	void GetColor(int &r, int &g, int &b, int &a) const
	{
		r = _color[0];
		g = _color[1];
		b = _color[2];
		a = _color[3];
	}

	void SetRawColor(int color32)
	{
		*((int *)this) = color32;
	}

	int GetRawColor() const
	{
		return *((int *)this);
	}

	inline int r() const { return _color[0]; }
	inline int g() const { return _color[1]; }
	inline int b() const { return _color[2]; }
	inline int a() const { return _color[3]; }

	inline float rBase() const { return _color[0] / 255.0f; }
	inline float gBase() const { return _color[1] / 255.0f; }
	inline float bBase() const { return _color[2] / 255.0f; }
	inline float aBase() const { return _color[3] / 255.0f; }

	void SetAlpha(int a) { _color[0] = (unsigned char)a; }
	int  GetAlpha() { return _color[0]; }


	unsigned char &operator[](int index)
	{
		return _color[index];
	}

	const unsigned char &operator[](int index) const
	{
		return _color[index];
	}

	bool operator == (const Color &rhs) const
	{
		return (*((int *)this) == *((int *)&rhs));
	}

	bool operator != (const Color &rhs) const
	{
		return !(operator==(rhs));
	}

	Color &operator=(const Color &rhs)
	{
		SetRawColor(rhs.GetRawColor());
		return *this;
	}

private:
	unsigned char _color[4];
};

template< typename Function > Function call_vfunc(PVOID Base, DWORD Index)
{
	PDWORD* VTablePointer = (PDWORD*)Base;
	PDWORD VTableFunctionBase = *VTablePointer;
	DWORD dwAddress = VTableFunctionBase[Index];
	return (Function)(dwAddress);
}



inline void setclantag(const char* tag)
{
	static auto ClanTagOffset = Utils::FindPattern("engine.dll", (PBYTE)"\x53\x56\x57\x8B\xDA\x8B\xF9\xFF\x15", "xxxxxxxxx");
	if (ClanTagOffset)
	{
		auto tag_ = std::string(tag);
		if (strlen(tag) > 0) {
			auto newline = tag_.find("\\n");
			auto tab = tag_.find("\\t");
			if (newline != std::string::npos) {
				tag_.replace(newline, newline + 2, "\n");
			}
			if (tab != std::string::npos) {
				tag_.replace(tab, tab + 2, "\t");
			}
		}
		static auto dankesttSetClanTag = reinterpret_cast<void(__fastcall*)(const char*, const char*)>(ClanTagOffset);
		dankesttSetClanTag(tag_.data(), tag_.data());
	}
}
namespace FONTS
{
	extern unsigned int menu_tab_font;
	extern unsigned int menu_checkbox_font;
	extern unsigned int menu_slider_font;
	extern unsigned int menu_groupbox_font;
	extern unsigned int menu_combobox_font;
	extern unsigned int menu_window_font;
	extern unsigned int watermarkfont;
	extern unsigned int numpad_menu_font;
	extern unsigned int visuals_esp_font;
	extern unsigned int visuals_xhair_font;
	extern unsigned int visuals_side_font;
	extern unsigned int visuals_name_font;
	extern unsigned int visuals_lby_font;
	
	bool ShouldReloadFonts();
	void InitFonts();
}

namespace SETTINGS
{
	class CSettings
	{
	public:
		bool Save(std::string file_name);
		bool Load(std::string file_name);

		void CreateConfig();

		std::vector<std::string> GetConfigs();

		bool bhop_bool;
		bool strafe_bool;
		bool fakewalk;
		bool moonwalk;
		bool esp_bool;
		int buybot;
		bool chatspam;
		bool namespam;
		bool astrowalk;
		float fov = 90;
		int chams_type;
		bool ragdoll;
		int xhair_type;
		bool xhair_bool;
		bool tp_bool;
		bool aim_bool;
		int aim_type;
		bool nohands;
		bool ArmMaterial;
		int misc_ArmMaterialsList;
		bool wirehands;
		int SmokeType;
		bool nigger;
		bool aa_bool;
		int weapon;
		bool nohandss;
		bool watermark;
		bool headshotonly;
		int acc_type;
		bool up_bool;
		bool knf_Active;
		bool knfActive;
		float knf_DistAttack;
		float knf_DistAttack2;
		int config_sel;
		bool beam_bool;
		int render_mode;
		bool fullbright;
		bool noflash;
		bool stop_bool;
		bool nosky;
		bool nopostprocess;
		bool esp_line;
		bool night_bool;
		bool box_bool;
		bool recoil_circle;
		bool name_bool;
		bool weap_bool;
		bool health_bool;
		bool skeleton_bool;
		bool drawfake_bool;
		bool showmisc;
		bool distance;
		int hitmarker_bool;
		bool info_bool;
		bool back_bool;
		bool hitmark_bool;
		
		bool lag_bool;
		int box_type;
		bool multi_bool;

		bool novisualrecoil_bool;

		bool multipoint_bool;
		bool auto_scope;
		bool sound_esp;
		float lag_choke;
		int clantag;

		bool debug_bool;

		CColor color;
		CColor spread_col;
		CColor manual_col;
		
		CColor vmodel_col;
		CColor imodel_col;
		CColor teamvmodel_col;
		CColor hitmarkerAlpha;
		CColor teamimodel_col;
		CColor selfvmodel_col;
		CColor selfimodel_col;


		CColor box_col;
		CColor friendbox_col;
		CColor selfbox_col;

		CColor skeleton_col;
		CColor localbeam_col;
		CColor enemybeam_col;
		CColor friendlybeam_col;
		CColor WorldMod_col;
		CColor PropMod_col;
		CColor fake_angle_col = CColor(255, 255, 255);
		CColor menu_col = CColor(240, 10, 10, 255);

		bool indicator_bool;

		float auto_chance_val;
		float auto_damage_val;

		float scout_damage_val;
		float scout_chance_val;

		float width;
		float height;

		float other_damage_val;
		float other_chance_val;
		bool fakefix_bool;
		float point_val;
		float body_val;

		bool flip_bool;
		int aa_side;

		int pitch_type;
		float NightStrength = 0.50;
		float LBYdelta;
		float AddReal;
		float AddFake;
		float FakeAngles = 0;
		float CustomPitch;
		float SpinSpeed = 50.0f;
		float JitterRange = 15;
		int fakeaa_type;
		int aa_type;

		int movingpitch_type;
		int realmove_type;
		int fakemove_type;
		float AddMovingReal;
		float AddMovingFake;
		float MovingFakeAngles = 0;
		float CustomMovingPitch;
		float MovingSpinSpeed = 50.0f;
		float MovingJitterRange = 15;

		int antiaimtype;

		bool localesp = false;
		bool teamesp = false;

	private:
	}; extern CSettings settings;
}

extern bool using_fake_angles[65];

extern bool in_tp;
extern bool fake_walk;

extern int resolve_type[65];

extern int shots_fired[65];
extern int shots_hit[65];
extern int shots_missed[65];

extern float tick_to_back[65];
extern float lby_to_back[65];
extern bool backtrack_tick[65];

extern float lby_delta;
extern float update_time[65];

extern int hitmarker_time;

extern bool menu_hide;

extern int oldest_tick[65];
extern float compensate[65][12];
extern Vector backtrack_hitbox[65][20][12];
extern float backtrack_simtime[65][12];

