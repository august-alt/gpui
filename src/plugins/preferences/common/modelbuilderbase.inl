namespace gpui
{

inline QStandardItem* ModelBuilderBase::itemCreator(const std::string& field)
{
    return new QStandardItem(QString::fromStdString(field));
}

inline QStandardItem* ModelBuilderBase::itemCreator(const unsigned char& field)
{
    return new QStandardItem(QString::number(static_cast<unsigned int>(field)));
}

template <typename T>
inline void ModelBuilderBase::createItemIfExist(const T& field, int index, int role,
                                                std::unique_ptr<QStandardItemModel>& model)
{
    if (field.present())
    {
        QStandardItem* categoryItem = itemCreator(field.get());
        model->setItem(index, role, categoryItem);
    }
}

template <typename T>
inline void ModelBuilderBase::createItem(T& field, int index,
                                         int role, std::unique_ptr<QStandardItemModel>& model)
{
    QStandardItem* categoryItem = itemCreator(field);
    model->setItem(index, role, categoryItem);
}

template <typename T>
inline void ModelBuilderBase::createComboBoxValue(T& field, int index, int role,
                                                  std::unique_ptr<QStandardItemModel>& model,
                                                  const QStringList& possibleValue)
{
    unsigned char currentIndex = 0;

    for (unsigned char i = 0; i < possibleValue.size(); ++i)
    {
        if (field.compare(possibleValue[i].toStdString()) == 0)
        {
            currentIndex = i + 1;
        }
    }

    createItem(currentIndex, index, role, model);
}

template <typename T>
inline void ModelBuilderBase::createBooleanValue(T& field, int index, int role,
                                                 std::unique_ptr<QStandardItemModel>& model,
                                                 bool invert)
{
    if (field.present())
    {
        bool state = invert ? !field.get() : field.get();

        QStandardItem* categoryItem = new QStandardItem(QString::number(static_cast<unsigned int>(state)));
        model->setItem(index, role, categoryItem);
    }
}

template <typename T>
inline void ModelBuilderBase::setStandardValues(T& preference, int index, std::unique_ptr<QStandardItemModel>& model)
{
    createItem(preference.name(), index, CommonRoles::NAME, model);

    createItem(preference.clsid(), index, CommonRoles::CLSID, model);

    createItemIfExist(preference.desc(), index, CommonRoles::DESC, model);

    createItemIfExist(preference.status(), index, CommonRoles::STATUS, model);

    createItemIfExist(preference.image(), index, CommonRoles::IMAGE, model);

    createItemIfExist(preference.changed(), index, CommonRoles::CHANGED, model);

    createItem(preference.uid(), index, CommonRoles::UID, model);

    createBooleanValue(preference.bypassErrors(), index, CommonRoles::BYPASS_ERRORS, model, true);

    createBooleanValue(preference.userContext(), index, CommonRoles::USER_CONTEXT, model);

    createBooleanValue(preference.removePolicy(), index, CommonRoles::REMOVE_POLICY, model);
}

}
