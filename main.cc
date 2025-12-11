//
#include <WinNe.h>

void a();

bool cksum(char* p, char* pe) {
	return nt::bytesum(p, pe) == *(unsigned char*)pe;
}

#ifdef _DEBUG
int main(int argc, char** argv)
#else
extern "C" int mainCRTStartup()
#endif
{
	nt::lopen();

	auto* cmd = NtCurrentPeb()->ProcessParameters->CommandLine.Buffer;
	if (*cmd == '"') {
		if (cmd = (wchar_t*)nt::wcsstr(cmd, L"\" ")) 
			cmd++;
	}
	else {
		cmd = (wchar_t*)nt::wcsstr(cmd, L" ");
	}
	if (!cmd++)
		return 1;

	estr f = nt::ereadf(cmd);
	if (f.empty())
		return 0;

	unsigned z=0, u=0;
	char* pf = f;
	bool b;

	const int args[] = {
		0x36C00,
		0x37200,
		0x37C00,
		0x5FE00,
		0x6F900,
		0x11B800,
		0x128A00,
		0x154000,
	};

	int& i = *(int*)&f[0x90036];
	//i = 175'000;
	b = cksum(&f[0x36C00], &f[0x37200-1]); //0
	nt::lg("55AA 0 - %d", b);
	b = cksum(&f[0x37200], &f[0x37C00 - 1]); /// 1
	nt::lg("55AA 1 - %d", b);
	b = cksum(&f[0x37C00], &f[0x5FE00 - 1]); // 1
	nt::lg("55AA 2 - %d", b);
	b = cksum(&f[0x5FE00], &f[0x6F900 - 1]); // 0
	nt::lg("55AA 3 - %d", b);
	b = cksum(&f[0x6F900], &f[0x11B800 - 1]); // 0
	nt::lg("55AA 4 - %d", b);
	b = cksum(&f[0x11B800], &f[0x128A00-1]); // 1
	nt::lg("55AA 5 - %d", b);
	b = cksum(&f[0x128A00], &f[0x154000 - 1]); // 1
	nt::lg("55AA 6 - %d", b);

	nt::lbr();
	nt::lg("Name %s", &f[0x390E0]); // (0x38ce0, 0x40));
	nt::lg("GPU %s", &f[0x3917C]); // (0x38d7c, 0x40));
	nt::lg("Bios %s[LOG]PartID %s", &f[0x39139], &f[0x39110 + 0x400]); // (0x38d39, 0x40));
	nt::lg("Date %s", &f[0x394EB]); // (0x36a42, 0x20));
	nt::lg("Vendor %s", &f[0x36A42 + 0x400]); // (0x390eb, 0x40));
	nt::lg("DeviceID %llX", nt::swapl((unsigned long long *)&f[0x38CA4 + 0x400])); // (0x38ca4, 0x38ca6));
	nt::lg("ImageChecksum %02X-%02X", nt::bytesum(&f[0x128A00], &f[0x154000 - 1]), QB(&f[0x154000 - 1])); // (0x11ae00, 0x1447ff));
	nt::lg("DefaultPL %dmW", Qi(&f[0x90032])); // (0x8fa82, 0x0));
	nt::lg("MaxPL %dmW", Qi(&f[0x90036])); // (0x8fa86, 0x0));
	nt::lg("MinPL %dmW", Qi(&f[0x9003A])); // (0x8fa8a, 0x0));
	//nt::lg("GPUClock %dMHz", Qi(&f[0x70492 + 0x400])); // (0x70492, 0x0));
	nt::lg("GPUClock %dMHz", Qi(&f[0x709BA])); // (0x70492, 0x0));
	nt::lg("PCIEPL %d",0 ); // (0x8fae6, 0x0));

	a();
	return 0;
}

/*
* 
* this.AddressMap = new Dictionary<string, GUI.Address>();
* this.AddressMap.Add("Name", ); // (0x38ce0, 0x40));
* this.AddressMap.Add("GPU", ); // (0x38d7c, 0x40));
* this.AddressMap.Add("Date", ); // (0x390eb, 0x40));
* this.AddressMap.Add("Bios", ); // (0x38d39, 0x40));
* this.AddressMap.Add("DeviceID", ); // (0x38ca4, 0x38ca6));
* this.AddressMap.Add("Vendor", ); // (0x36a42, 0x20));
* this.AddressMap.Add("ImageChecksum", ); // (0x11ae00, 0x1447ff));
* this.AddressMap.Add("DefaultPL", ); // (0x8fa86, 0x0));
* this.AddressMap.Add("MaxPL", ); // (0x8fa8a, 0x0));
* this.AddressMap.Add("MinPL", ); // (0x8fa82, 0x0));
* this.AddressMap.Add("GPUClock", ); // (0x70492, 0x0));
* this.AddressMap.Add("PCIEPL", ); // (0x8fae6, 0x0));
* this.AddressMap.Add("PCBConfig", ); // (0x39110, 0x8));
*
*/
