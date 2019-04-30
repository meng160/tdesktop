/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#pragma once

#include "dialogs/dialogs_entry.h"
#include "dialogs/dialogs_main_list.h"
#include "data/data_messages.h"

class ChannelData;
class AuthSession;

namespace Data {

class Session;
class Folder;

//MessagePosition FeedPositionFromMTP(const MTPFeedPosition &position); // #feed

class Folder final : public Dialogs::Entry {
public:
	static constexpr auto kId = 1;

	Folder(not_null<Data::Session*> owner, FolderId id);
	Folder(const Folder &) = delete;
	Folder &operator=(const Folder &) = delete;

	FolderId id() const;
	void registerOne(not_null<History*> history);
	void unregisterOne(not_null<History*> history);
	void oneListMessageChanged(HistoryItem *from, HistoryItem *to);

	not_null<Dialogs::MainList*> chatsList();

	void applyDialog(const MTPDdialogFolder &data);
	void applyPinnedUpdate(const MTPDupdateDialogPinned &data);

	//MessagePosition unreadPosition() const; // #feed
	//rpl::producer<MessagePosition> unreadPositionChanges() const; // #feed

	void updateCloudUnread(const MTPDdialogFolder &data);
	void unreadStateChanged(
		const Dialogs::Key &key,
		const Dialogs::UnreadState &wasState,
		const Dialogs::UnreadState &nowState);
	void unreadEntryChanged(
		const Dialogs::Key &key,
		const Dialogs::UnreadState &state,
		bool added);

	TimeId adjustedChatListTimeId() const override;

	int fixedOnTopIndex() const override;
	bool toImportant() const override;
	bool shouldBeInChatList() const override;
	int chatListUnreadCount() const override;
	bool chatListUnreadMark() const override;
	bool chatListMutedBadge() const override;
	Dialogs::UnreadState chatListUnreadState() const override;
	HistoryItem *chatListMessage() const override;
	bool chatListMessageKnown() const override;
	void requestChatListMessage() override;
	const QString &chatListName() const override;
	const base::flat_set<QString> &chatListNameWords() const override;
	const base::flat_set<QChar> &chatListFirstLetters() const override;

	void loadUserpic() override;
	void paintUserpic(
		Painter &p,
		int x,
		int y,
		int size) const override;

	bool chatsListLoaded() const;
	void setChatsListLoaded(bool loaded = true);
	void setCloudChatsListSize(int size);

	int chatsListSize() const;
	int unreadHistoriesCount() const;
	const std::vector<not_null<History*>> &lastUnreadHistories() const;
	uint32 chatListViewVersion() const;

private:
	void indexNameParts();
	bool applyChatListMessage(HistoryItem *item);
	void computeChatListMessage();

	void addUnreadHistory(not_null<History*> history);
	void removeUnreadHistory(not_null<History*> history);
	void reorderUnreadHistories();
	void finalizeCloudUnread();

	FolderId _id = 0;
	Dialogs::MainList _chatsList;

	QString _name;
	base::flat_set<QString> _nameWords;
	base::flat_set<QChar> _nameFirstLetters;

	Dialogs::UnreadState _cloudUnread;
	int _cloudChatsListSize = 0;
	std::vector<not_null<History*>> _unreadHistories;
	std::vector<not_null<History*>> _unreadHistoriesLast;
	HistoryItem *_chatListMessage = nullptr;
	uint32 _chatListViewVersion = 0;
	//rpl::variable<MessagePosition> _unreadPosition;

	rpl::lifetime _lifetime;

};

} // namespace Data