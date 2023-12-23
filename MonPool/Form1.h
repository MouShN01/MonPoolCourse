#pragma once
#include "MtDll.h"

#pragma comment(lib, "ntdll")

struct TagItem {
	SYSTEM_POOLTAG TagInfo;
	CStringA SourceName = L"";
	CStringA SourceDesc = L"";
	CStringA Tag;
	int Index;
};

size_t current_info = 0;
size_t m_TotalPaged = 0, m_TotalNonPaged = 0;
std::map<CStringA, std::pair<CString, CString>> m_TagSource;
std::unordered_map<ULONG, std::shared_ptr<TagItem>> m_TagsMap;
std::vector<std::shared_ptr<TagItem>> m_Tags;
SYSTEM_POOLTAG_INFORMATION* m_PoolTags{ nullptr };

bool sorted0 = 1;
bool sorted1 = 1;
bool sorted2 = 1;
bool sorted3 = 1;
bool sorted4 = 1;
bool sorted5 = 1;
bool sorted6 = 1;
bool sorted7 = 1;

bool compareByTag(const std::shared_ptr<TagItem>& a, const std::shared_ptr<TagItem>& b) {
	if (!sorted0)
	{
		return a->Tag < b->Tag;
	}
	else
	{
		return a->Tag > b->Tag;
	}
}

auto compareSharedPtrByPagedAllocs = [](const std::shared_ptr<TagItem>& a, const std::shared_ptr<TagItem>& b) {
	if (!sorted1)
	{
		return a->TagInfo.PagedAllocs > b->TagInfo.PagedAllocs;
	}
	else
	{
		return a->TagInfo.PagedAllocs < b->TagInfo.PagedAllocs;
	}
};

auto compareSharedPtrByPagedFrees = [](const std::shared_ptr<TagItem>& a, const std::shared_ptr<TagItem>& b) {
	if (!sorted2)
	{
		return a->TagInfo.PagedFrees > b->TagInfo.PagedFrees;
	}
	else
	{
		return a->TagInfo.PagedFrees < b->TagInfo.PagedFrees;
	}

};

auto compareSharedPtrByDiff = [](const std::shared_ptr<TagItem>& a, const std::shared_ptr<TagItem>& b) {
	if (!sorted3)
	{
		return a->TagInfo.PagedAllocs - a->TagInfo.PagedFrees > b->TagInfo.PagedAllocs - b->TagInfo.PagedFrees;
	}
	else
	{
		return a->TagInfo.PagedAllocs - a->TagInfo.PagedFrees < b->TagInfo.PagedAllocs - b->TagInfo.PagedFrees;
	}
};

auto compareSharedPtrByNPagedAllocs = [](const std::shared_ptr<TagItem>& a, const std::shared_ptr<TagItem>& b) {
	if (!sorted4)
	{
		return a->TagInfo.NonPagedAllocs > b->TagInfo.NonPagedAllocs;
	}
	else
	{
		return a->TagInfo.NonPagedAllocs < b->TagInfo.NonPagedAllocs;
	}
};

auto compareSharedPtrByNPagedFrees = [](const std::shared_ptr<TagItem>& a, const std::shared_ptr<TagItem>& b) {
	if (!sorted5)
	{
		return a->TagInfo.NonPagedFrees > b->TagInfo.NonPagedFrees;
	}
	else
	{
		return a->TagInfo.NonPagedFrees < b->TagInfo.NonPagedFrees;
	}
};

auto compareSharedPtrByNDiff = [](const std::shared_ptr<TagItem>& a, const std::shared_ptr<TagItem>& b) {
	if (!sorted6)
	{
		return a->TagInfo.NonPagedAllocs - a->TagInfo.NonPagedFrees > b->TagInfo.NonPagedAllocs - b->TagInfo.NonPagedFrees;
	}
	else
	{
		return a->TagInfo.NonPagedAllocs - a->TagInfo.NonPagedFrees < b->TagInfo.NonPagedAllocs - b->TagInfo.NonPagedFrees;
	}
};

bool compareBySourceName(const std::shared_ptr<TagItem>& a, const std::shared_ptr<TagItem>& b) {
	if (!sorted7)
	{
		return a->SourceName < b->SourceName;
	}
	else
	{
		return a->SourceName > b->SourceName;
	}
}

System::String^ ConvertCStringAToString(const ATL::CStringA& cstringA)
{
	const char* charArray = static_cast<const char*>(cstringA);

	return gcnew System::String(charArray);
}

void AddTag(const SYSTEM_POOLTAG& info, int index) {
	char tag[5] = { 0 };
	::CopyMemory(tag, &info.Tag, 4);
	auto item = std::make_shared<TagItem>();
	item->Tag = CStringA(tag);

	item->TagInfo = info;
	item->Index = index;

	if (auto it = m_TagSource.find(item->Tag); it != m_TagSource.end()) {
		item->SourceName = it->second.first;
		item->SourceDesc = it->second.second;
	}

	m_Tags.push_back(item);
	m_TagsMap.insert({ info.TagUlong, item });
}

namespace CppCLRWinFormsProject {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
		}

	protected:
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::ListView^ listView;
	protected:

	private: System::Windows::Forms::ColumnHeader^ tagCol;
	private: System::Windows::Forms::ColumnHeader^ pAllocsCol;
	private: System::Windows::Forms::ColumnHeader^ pFreesCol;
	private: System::Windows::Forms::ColumnHeader^ pDiffCol;
	private: System::Windows::Forms::ColumnHeader^ npAllocsCol;
	private: System::Windows::Forms::ColumnHeader^ npFreesCol;
	private: System::Windows::Forms::ColumnHeader^ npDiffCol;
	private: System::Windows::Forms::ColumnHeader^ nameCol;
	private: System::Windows::Forms::ColumnHeader^ descCol;
	private: System::Windows::Forms::Timer^ timer1;
	private: System::Windows::Forms::MenuStrip^ menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^ controlsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ stopToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ continueToolStripMenuItem;

	private: System::ComponentModel::IContainer^ components;
	protected:

	private:

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->listView = (gcnew System::Windows::Forms::ListView());
			this->tagCol = (gcnew System::Windows::Forms::ColumnHeader());
			this->pAllocsCol = (gcnew System::Windows::Forms::ColumnHeader());
			this->pFreesCol = (gcnew System::Windows::Forms::ColumnHeader());
			this->pDiffCol = (gcnew System::Windows::Forms::ColumnHeader());
			this->npAllocsCol = (gcnew System::Windows::Forms::ColumnHeader());
			this->npFreesCol = (gcnew System::Windows::Forms::ColumnHeader());
			this->npDiffCol = (gcnew System::Windows::Forms::ColumnHeader());
			this->nameCol = (gcnew System::Windows::Forms::ColumnHeader());
			this->descCol = (gcnew System::Windows::Forms::ColumnHeader());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->controlsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->stopToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->continueToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// listView
			// 
			this->listView->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(9) {
				this->tagCol, this->pAllocsCol,
					this->pFreesCol, this->pDiffCol, this->npAllocsCol, this->npFreesCol, this->npDiffCol, this->nameCol, this->descCol
			});
			this->listView->Dock = System::Windows::Forms::DockStyle::Fill;
			this->listView->GridLines = true;
			this->listView->HideSelection = false;
			this->listView->Location = System::Drawing::Point(0, 30);
			this->listView->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->listView->Name = L"listView";
			this->listView->Size = System::Drawing::Size(1049, 753);
			this->listView->TabIndex = 0;
			this->listView->UseCompatibleStateImageBehavior = false;
			this->listView->View = System::Windows::Forms::View::Details;
			this->listView->ColumnClick += gcnew System::Windows::Forms::ColumnClickEventHandler(this, &Form1::listView_ColumnClick);
			// 
			// tagCol
			// 
			this->tagCol->Text = L"Tag";
			this->tagCol->Width = 40;
			// 
			// pAllocsCol
			// 
			this->pAllocsCol->Text = L"Paged Allocs";
			this->pAllocsCol->Width = 108;
			// 
			// pFreesCol
			// 
			this->pFreesCol->Text = L"Paged Frees";
			this->pFreesCol->Width = 102;
			// 
			// pDiffCol
			// 
			this->pDiffCol->Text = L"Paged Diff";
			this->pDiffCol->Width = 89;
			// 
			// npAllocsCol
			// 
			this->npAllocsCol->Text = L"NPaged Allocs";
			this->npAllocsCol->Width = 112;
			// 
			// npFreesCol
			// 
			this->npFreesCol->Text = L"NPaged Frees";
			this->npFreesCol->Width = 113;
			// 
			// npDiffCol
			// 
			this->npDiffCol->Text = L"NPaged Diff";
			this->npDiffCol->Width = 103;
			// 
			// nameCol
			// 
			this->nameCol->Text = L"Name";
			this->nameCol->Width = 75;
			// 
			// descCol
			// 
			this->descCol->Text = L"Description";
			this->descCol->Width = 500;
			// 
			// timer1
			// 
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// menuStrip1
			// 
			this->menuStrip1->ImageScalingSize = System::Drawing::Size(20, 20);
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->controlsToolStripMenuItem });
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(1049, 30);
			this->menuStrip1->TabIndex = 1;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// controlsToolStripMenuItem
			// 
			this->controlsToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->stopToolStripMenuItem,
					this->continueToolStripMenuItem
			});
			this->controlsToolStripMenuItem->Name = L"controlsToolStripMenuItem";
			this->controlsToolStripMenuItem->Size = System::Drawing::Size(78, 26);
			this->controlsToolStripMenuItem->Text = L"Controls";
			// 
			// stopToolStripMenuItem
			// 
			this->stopToolStripMenuItem->Name = L"stopToolStripMenuItem";
			this->stopToolStripMenuItem->Size = System::Drawing::Size(224, 26);
			this->stopToolStripMenuItem->Text = L"Stop";
			this->stopToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::stopToolStripMenuItem_Click);
			// 
			// continueToolStripMenuItem
			// 
			this->continueToolStripMenuItem->Name = L"continueToolStripMenuItem";
			this->continueToolStripMenuItem->Size = System::Drawing::Size(224, 26);
			this->continueToolStripMenuItem->Text = L"Continue";
			this->continueToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::continueToolStripMenuItem_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1049, 783);
			this->Controls->Add(this->listView);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->Name = L"Form1";
			this->Text = L"Pool Monitoring";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}

		void LoadPoolTagText() {

			std::ifstream inFile("pooltag.txt");

			// Проверяем, удалось ли открыть файл для чтения
			if (!inFile.is_open()) {
				std::cout << "Не удалось открыть файл для чтения" << std::endl;
			}

			// Определяем размер файла
			inFile.seekg(0, std::ios::end);
			std::streamsize fileSize = inFile.tellg();
			inFile.seekg(0, std::ios::beg);

			// Выделяем буфер для считывания содержимого файла
			char* buffer = new char[fileSize + 1];

			// Считываем содержимое файла в буфер
			inFile.read(buffer, fileSize);

			// Завершаем строку null-терминатором
			buffer[fileSize] = '\0';

			// Закрываем файл
			inFile.close();


			auto data = static_cast<const char*>(buffer);
			auto next = strchr(data, '\n');
			for (; next; data = next + 1) {
				if (strncmp(data, "//", 2) == 0 || _strnicmp(data, "rem", 3) == 0
					|| strncmp(data, "\r\n", 2) == 0 || strncmp(data, "\n", 1) == 0) {
					next = strchr(data, '\n');
					continue;
				}

				next = strchr(data, '\n');

				// read the tag
				std::string tag(data, data + 4);

				// locate the first dash
				auto dash1 = strchr(data, '-');
				if (dash1 == nullptr)
					continue;

				// locate second dash
				auto dash2 = strchr(dash1 + 1, '-');
				if (dash2 == nullptr)
					continue;

				if (dash2 > next) {
					dash2 = dash1;
					dash1 = nullptr;
				}

				CStringA trimmedTag(tag.c_str());
				trimmedTag.TrimLeft();
				trimmedTag += L"  ";
				if (trimmedTag.GetLength() > 4)
					trimmedTag = trimmedTag.Mid(0, 4);
				CStringW trimmedDriverName(L"");
				if (dash1) {
					std::string driverName(dash1 + 1, dash2);
					trimmedDriverName = driverName.c_str();
					trimmedDriverName.Trim();
				}

				std::string driverDesc(dash2 + 1, next - 1);
				CStringW trimmedDriverDesc(driverDesc.c_str());
				trimmedDriverDesc.Trim();

				m_TagSource.insert({ trimmedTag, std::make_pair(CString(trimmedDriverName), CString(trimmedDriverDesc)) });
			}
		}

		void UpdatePoolTags() {
			ULONG size = 1 << 22;
			if (m_PoolTags == nullptr) {
				LoadPoolTagText();
				m_PoolTags = static_cast<SYSTEM_POOLTAG_INFORMATION*>(::VirtualAlloc(nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));
				if (m_PoolTags == nullptr) {
					std::cout << "Not enough memory" << std::endl;
					PostQuitMessage(1);
					return;
				}
			}
			ULONG len;
			auto status = NtQuerySystemInformation(SystemInformationClass::SystemPoolTagInformation, m_PoolTags, size, &len);

			if (status) {
				std::cout << "Failed in getting pool information" << std::endl;
				PostQuitMessage(1);
				return;
			}

			auto count = m_PoolTags->Count;
			if (m_Tags.empty()) {
				m_Tags.reserve(count + 16);
				m_TagsMap.reserve(count + 16);

				for (decltype(count) i = 0; i < count; i++) {
					const auto& info = m_PoolTags->TagInfo[i];
					m_TotalPaged += info.PagedUsed;
					m_TotalNonPaged += info.NonPagedUsed;
					AddTag(info, i);
				}
			}
			else {
				int size = static_cast<int>(m_Tags.size());
				std::unordered_set<int> set;
				for (int i = 0; i < size; i++)
					set.insert(i);

				m_TotalPaged = m_TotalNonPaged = 0;
				for (decltype(count) i = 0; i < count; i++) {
					const auto& info = m_PoolTags->TagInfo[i];

					m_TotalPaged += info.PagedUsed;
					m_TotalNonPaged += info.NonPagedUsed;

					auto it = m_TagsMap.find(info.TagUlong);
					if (it == m_TagsMap.end()) {
						AddTag(info, i);
						count++;
					}
					else {
						auto& newinfo = it->second->TagInfo;

						it->second->TagInfo = info;
						it->second->Index = i;
						set.erase(i);
					}
				}

				int bias = 0;
				for (auto index : set) {
					int i = index - bias;
					count--;
					m_TagsMap.erase(m_Tags[i]->TagInfo.TagUlong);
					m_Tags.erase(m_Tags.begin() + i);
					bias++;
				}
			}
		}

		void CreateList()
		{
			for (int i = 0; i < m_Tags.size(); i++)
			{
				auto& info = *m_Tags[i];
				System::String^ resultString = ConvertCStringAToString(info.Tag);
				System::String^ allocString = System::UInt32(info.TagInfo.PagedAllocs).ToString();
				System::String^ freesString = System::UInt32(info.TagInfo.PagedFrees).ToString();
				System::String^ diffString = System::UInt32(info.TagInfo.PagedAllocs - info.TagInfo.PagedFrees).ToString();
				System::String^ nAllocString = System::UInt32(info.TagInfo.NonPagedAllocs).ToString();
				System::String^ nFreesString =System::UInt32(info.TagInfo.NonPagedFrees).ToString();
				System::String^ nDiffString = System::UInt32(info.TagInfo.NonPagedAllocs - info.TagInfo.NonPagedFrees).ToString();
				System::String^ name = ConvertCStringAToString(info.SourceName);
				System::String^ desc = ConvertCStringAToString(info.SourceDesc);


				ListViewItem^ item = gcnew ListViewItem(resultString);
				item->SubItems->Add(allocString);
				item->SubItems->Add(freesString);
				item->SubItems->Add(diffString);
				item->SubItems->Add(nAllocString);
				item->SubItems->Add(nFreesString);
				item->SubItems->Add(nDiffString);
				item->SubItems->Add(name);
				item->SubItems->Add(desc);
				listView->Items->Add(item);
			}
		}

		void UpdateList()
		{
			int per = 100;
			int start = current_info;
			for(int i = start; i < start + per && i < listView->Items->Count; i++)
			{
				auto& info = *m_Tags[i];
				listView->Items[i]->SubItems[0]->Text = ConvertCStringAToString(info.Tag);
				listView->Items[i]->SubItems[1]->Text = System::UInt32(info.TagInfo.PagedAllocs).ToString();
				listView->Items[i]->SubItems[2]->Text = System::UInt32(info.TagInfo.PagedFrees).ToString();
				listView->Items[i]->SubItems[3]->Text = System::UInt32(info.TagInfo.PagedAllocs - info.TagInfo.PagedFrees).ToString();
				listView->Items[i]->SubItems[4]->Text = System::UInt32(info.TagInfo.NonPagedAllocs).ToString();
				listView->Items[i]->SubItems[5]->Text = System::UInt32(info.TagInfo.NonPagedFrees).ToString();
				listView->Items[i]->SubItems[6]->Text = System::UInt32(info.TagInfo.NonPagedAllocs - info.TagInfo.NonPagedFrees).ToString();
				listView->Items[i]->SubItems[7]->Text = ConvertCStringAToString(info.SourceName);
				listView->Items[i]->SubItems[8]->Text = ConvertCStringAToString(info.SourceDesc);
			}
			current_info = start + per;
			if (current_info >= listView->Items->Count)
			{
				current_info = 0;
			}
		}

		void UpdateListOnce()
		{
			for (int i = 0; i < listView->Items->Count; i++)
			{
				auto& info = *m_Tags[i];
				listView->Items[i]->SubItems[0]->Text = ConvertCStringAToString(info.Tag);
				listView->Items[i]->SubItems[1]->Text = System::UInt32(info.TagInfo.PagedAllocs).ToString();
				listView->Items[i]->SubItems[2]->Text = System::UInt32(info.TagInfo.PagedFrees).ToString();
				listView->Items[i]->SubItems[3]->Text = System::UInt32(info.TagInfo.PagedAllocs - info.TagInfo.PagedFrees).ToString();
				listView->Items[i]->SubItems[4]->Text = System::UInt32(info.TagInfo.NonPagedAllocs).ToString();
				listView->Items[i]->SubItems[5]->Text = System::UInt32(info.TagInfo.NonPagedFrees).ToString();
				listView->Items[i]->SubItems[6]->Text = System::UInt32(info.TagInfo.NonPagedAllocs - info.TagInfo.NonPagedFrees).ToString();
				listView->Items[i]->SubItems[7]->Text = ConvertCStringAToString(info.SourceName);
				listView->Items[i]->SubItems[8]->Text = ConvertCStringAToString(info.SourceDesc);
			}
		}

#pragma endregion
	private: System::Void Form1_Load(System::Object^ sender, System::EventArgs^ e) 
	{

		LoadPoolTagText();
		UpdatePoolTags();
		CreateList();
		timer1->Start();
	}
	private: System::Void timer1_Tick(System::Object^ sender, System::EventArgs^ e) 
	{
		UpdatePoolTags();
		UpdateList();
	}

	private: System::Void listView_ColumnClick(System::Object^ sender, System::Windows::Forms::ColumnClickEventArgs^ e) {
		switch (e->Column)
		{
		case 0:
			if (sorted0)
			{
				sorted0 = 0;
				sorted1 = 1;
				sorted2 = 1;
				sorted3 = 1;
				sorted4 = 1;
				sorted5 = 1;
				sorted6 = 1;
				sorted7 = 1;
			}
			else
			{
				sorted0 = 1;
				sorted1 = 0;
				sorted2 = 0;
				sorted3 = 0;
				sorted4 = 0;
				sorted5 = 0;
				sorted6 = 0;
				sorted7 = 0;
			}
			std::sort(m_Tags.begin(), m_Tags.end(), compareByTag);
			break;
		case 1:
			if (sorted1)
			{
				sorted0 = 1;
				sorted1 = 0;
				sorted2 = 1;
				sorted3 = 1;
				sorted4 = 1;
				sorted5 = 1;
				sorted6 = 1;
				sorted7 = 1;
			}
			else
			{
				sorted0 = 0;
				sorted1 = 1;
				sorted2 = 0;
				sorted3 = 0;
				sorted4 = 0;
				sorted5 = 0;
				sorted6 = 0;
				sorted7 = 0;
			}
			std::sort(m_Tags.begin(), m_Tags.end(), compareSharedPtrByPagedAllocs);
			break;
		case 2:
			if (sorted2)
			{
				sorted0 = 1;
				sorted1 = 1;
				sorted2 = 0;
				sorted3 = 1;
				sorted4 = 1;
				sorted5 = 1;
				sorted6 = 1;
				sorted7 = 1;
			}
			else
			{
				sorted0 = 0;
				sorted1 = 0;
				sorted2 = 1;
				sorted3 = 0;
				sorted4 = 0;
				sorted5 = 0;
				sorted6 = 0;
				sorted7 = 0;
			}
			std::sort(m_Tags.begin(), m_Tags.end(), compareSharedPtrByPagedFrees);
			break;
		case 3:
			if (sorted3)
			{
				sorted0 = 1;
				sorted1 = 1;
				sorted2 = 1;
				sorted3 = 0;
				sorted4 = 1;
				sorted5 = 1;
				sorted6 = 1;
				sorted7 = 1;
			}
			else
			{
				sorted0 = 0;
				sorted1 = 0;
				sorted2 = 0;
				sorted3 = 1;
				sorted4 = 0;
				sorted5 = 0;
				sorted6 = 0;
				sorted7 = 0;
			}
			std::sort(m_Tags.begin(), m_Tags.end(), compareSharedPtrByDiff);
			break;
		case 4:
			if (sorted4)
			{
				sorted0 = 1;
				sorted1 = 1;
				sorted2 = 1;
				sorted3 = 1;
				sorted4 = 0;
				sorted5 = 1;
				sorted6 = 1;
				sorted7 = 1;
			}
			else
			{
				sorted0 = 0;
				sorted1 = 0;
				sorted2 = 0;
				sorted3 = 0;
				sorted4 = 1;
				sorted5 = 0;
				sorted6 = 0;
				sorted7 = 0;
			}
			std::sort(m_Tags.begin(), m_Tags.end(), compareSharedPtrByNPagedAllocs);
			break;
		case 5:
			if (sorted5)
			{
				sorted0 = 1;
				sorted1 = 1;
				sorted2 = 1;
				sorted3 = 1;
				sorted4 = 1;
				sorted5 = 0;
				sorted6 = 1;
				sorted7 = 1;
			}
			else
			{
				sorted0 = 0;
				sorted1 = 0;
				sorted2 = 0;
				sorted3 = 0;
				sorted4 = 0;
				sorted5 = 1;
				sorted6 = 0;
				sorted7 = 0;
			}
			std::sort(m_Tags.begin(), m_Tags.end(), compareSharedPtrByNPagedFrees);
			break;
		case 6:
			if (sorted6)
			{
				sorted0 = 1;
				sorted1 = 1;
				sorted2 = 1;
				sorted3 = 1;
				sorted4 = 1;
				sorted5 = 1;
				sorted6 = 0;
				sorted7 = 1;
			}
			else
			{
				sorted0 = 0;
				sorted1 = 0;
				sorted2 = 0;
				sorted3 = 0;
				sorted4 = 0;
				sorted5 = 0;
				sorted6 = 1;
				sorted7 = 0;
			}
			std::sort(m_Tags.begin(), m_Tags.end(), compareSharedPtrByNDiff);
			break;
		case 7:
			if (sorted7)
			{
				sorted0 = 1;
				sorted1 = 1;
				sorted2 = 1;
				sorted3 = 1;
				sorted4 = 1;
				sorted5 = 1;
				sorted6 = 1;
				sorted7 = 0;
			}
			else
			{
				sorted0 = 0;
				sorted1 = 0;
				sorted2 = 0;
				sorted3 = 0;
				sorted4 = 0;
				sorted5 = 0;
				sorted6 = 0;
				sorted7 = 1;
			}
			std::sort(m_Tags.begin(), m_Tags.end(), compareBySourceName);
			break;
		}
		UpdatePoolTags();
		UpdateListOnce();
	}
	private: System::Void stopToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		timer1->Stop();
	}
	private: System::Void continueToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		timer1->Start();
	}
};
}