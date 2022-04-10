// Pegasus Frontend
// Copyright (C) 2017-2022  Mátyás Mustoha
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.


#include "GameListModel.h"

#include "model/gaming/Assets.h"
#include "model/gaming/Collection.h"
#include "model/gaming/Game.h"
#include "model/gaming/GameFile.h"


namespace model {
enum Roles {
    Self = Qt::UserRole,
    Title,
    SortBy,
    Summary,
    Description,
    Players,
    Rating,
    Release,
    ReleaseYear,
    ReleaseMonth,
    ReleaseDay,
    PlayCount,
    PlayTime,
    LastPlayed,
    Favorite,
    Extra,
    Developer,
    DeveloperList,
    Publisher,
    PublisherList,
    Genre,
    GenreList,
    Tag,
    TagList,
    Assets,
    Files,
    Collections,
};


GameListModel::GameListModel(QObject* parent)
    : QAbstractListModel(parent)
{}


QHash<int, QByteArray> GameListModel::roleNames() const
{
    static QHash<int, QByteArray> ROLE_NAMES {
        { Roles::Self, QByteArrayLiteral("modelData") },
        { Roles::Title, QByteArrayLiteral("title") },
        { Roles::SortBy, QByteArrayLiteral("sortBy") },
        { Roles::Summary, QByteArrayLiteral("summary") },
        { Roles::Description, QByteArrayLiteral("description") },
        { Roles::Release, QByteArrayLiteral("release") },
        { Roles::Players, QByteArrayLiteral("players") },
        { Roles::Rating, QByteArrayLiteral("rating") },
        { Roles::ReleaseYear, QByteArrayLiteral("releaseYear") },
        { Roles::ReleaseMonth, QByteArrayLiteral("releaseMonth") },
        { Roles::ReleaseDay, QByteArrayLiteral("releaseDay") },
        { Roles::PlayCount, QByteArrayLiteral("playCount") },
        { Roles::PlayTime, QByteArrayLiteral("playTime") },
        { Roles::LastPlayed, QByteArrayLiteral("lastPlayed") },
        { Roles::Favorite, QByteArrayLiteral("favorite") },
        { Roles::Extra, QByteArrayLiteral("extra") },
        { Roles::Developer, QByteArrayLiteral("developer") },
        { Roles::DeveloperList, QByteArrayLiteral("developerList") },
        { Roles::Publisher, QByteArrayLiteral("publisher") },
        { Roles::PublisherList, QByteArrayLiteral("publisherList") },
        { Roles::Genre, QByteArrayLiteral("genre") },
        { Roles::GenreList, QByteArrayLiteral("genreList") },
        { Roles::Tag, QByteArrayLiteral("tag") },
        { Roles::TagList, QByteArrayLiteral("tagList") },
        { Roles::Assets, QByteArrayLiteral("assets") },
        { Roles::Files, QByteArrayLiteral("files") },
        { Roles::Collections, QByteArrayLiteral("collections") },
    };
    return ROLE_NAMES;
}


GameListModel& GameListModel::update(std::vector<model::Game*>&& entries)
{
    const bool count_changed = m_entries.size() != entries.size();

    beginResetModel();
    for (model::Game* game : m_entries)
        disconnect(game, nullptr, this, nullptr);

    m_entries = std::move(entries);

    for (model::Game* game : m_entries) {
        connect(game, &model::Game::favoriteChanged,
                this, [this](){ onGamePropertyChanged({Roles::Favorite}); });
        connect(game, &model::Game::playStatsChanged,
                this, [this](){ onGamePropertyChanged({Roles::PlayCount, Roles::PlayTime, Roles::LastPlayed}); });
    }
    endResetModel();

    if (count_changed)
        emit countChanged();

    return *this;
}


int GameListModel::rowCount(const QModelIndex& parent) const
{
    return parent.isValid() ? 0 : m_entries.size();
}


QVariant GameListModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return {};

    model::Game* const game_ptr = m_entries.at(index.row());
    const model::Game& game = *game_ptr;
    switch (role) {
        case Roles::Self: return QVariant::fromValue(game_ptr);
        case Roles::Title: return game.title();
        case Roles::SortBy: return game.sortBy();
        case Roles::Summary: return game.summary();
        case Roles::Description: return game.description();
        case Roles::Players: return game.playerCount();
        case Roles::Rating: return game.rating();
        case Roles::Release: return game.releaseDate();
        case Roles::ReleaseYear: return game.releaseYear();
        case Roles::ReleaseMonth: return game.releaseMonth();
        case Roles::ReleaseDay: return game.releaseDay();
        case Roles::PlayCount: return game.playCount();
        case Roles::PlayTime: return game.playTime();
        case Roles::LastPlayed: return game.lastPlayed();
        case Roles::Favorite: return game.isFavorite();
        case Roles::Extra: return game.extraMap();
        case Roles::Developer: return game.developerStr();
        case Roles::DeveloperList: return game.developerListConst();
        case Roles::Publisher: return game.publisherStr();
        case Roles::PublisherList: return game.publisherListConst();
        case Roles::Genre: return game.genreStr();
        case Roles::GenreList: return game.genreListConst();
        case Roles::Tag: return game.tagStr();
        case Roles::TagList: return game.tagListConst();
        case Roles::Assets: return QVariant::fromValue(game.assetsPtr());
        case Roles::Files: return QVariant::fromValue(game.files());
        case Roles::Collections: return QVariant::fromValue(game.collectionsModel());
        default: return {};
    }
}


QVariantList GameListModel::toVarArray() const
{
    QVariantList varlist;
    varlist.reserve(m_entries.size());
    for (model::Game* ptr : m_entries)
        varlist.append(QVariant::fromValue(ptr));
    return varlist;
}


model::Game* GameListModel::get(int idx) const
{
    return (0 <= idx && static_cast<size_t>(idx) < m_entries.size())
        ? m_entries.at(idx)
        : nullptr;
}


void GameListModel::onGamePropertyChanged(const QVector<int>& roles)
{
    QObject* const game_ptr = sender();
    const auto it = std::find(m_entries.cbegin(), m_entries.cend(), game_ptr);
    if (it == m_entries.cend())
       return;

    const size_t data_idx = std::distance(m_entries.cbegin(), it);
    const QModelIndex model_idx = index(data_idx);
    emit dataChanged(model_idx, model_idx, roles);
}
} // namespace model