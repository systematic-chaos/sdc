import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.ticker import MultipleLocator
from os import path, mkdir

data_path = path.abspath(path.join('data', 'lab2_data.xlsx'))

figures_path = 'images'
if not path.exists(figures_path):
    mkdir(figures_path, mode=0o740)



def load_data(data_sheet_path, data_sheet_index=0):
    return pd.read_excel(data_sheet_path,\
        sheet_name=data_sheet_index, index_col=0, parse_dates=True, decimal=',')

def save_figure(fig_name, ext='jpg'):
    plt.savefig(path.join(figures_path, fig_name + '.' + ext))

def valencian_rainfall(cv_rain_energy_price_df, save_plot=True):
    # Extract data of rainfall in the Valencian Community
    rainfall_series = cv_rain_energy_price_df.iloc[:, 0]

    # Initialize and customize bar plot axis
    plt.figure()
    axis = rainfall_series.plot.bar(title=rainfall_series.name,\
        color='tab:blue', ylim=[0, 180])
    axis.set_xticklabels([ label.strftime('%b-%Y') for label in rainfall_series.index ],\
        rotation=90, horizontalalignment='right')
    axis.get_yaxis().set_major_locator(MultipleLocator(20))
    axis.get_yaxis().set_tick_params(color='lightgray')
    axis.grid(visible=True, axis='y', which='major', color='lightgray')
    axis.spines['top'].set(visible=False)
    axis.spines['left'].set(visible=False)
    axis.spines['right'].set(visible=False)

    # Store the plot's figure and return the resulting dataset
    if save_plot:
        save_figure('fig1')
    return rainfall_series

def energy_price(cv_rain_energy_price_df, save_plot=True):
    # Extract data of energy price
    price_series = cv_rain_energy_price_df.iloc[:, 1]

    # Initialize and customize bar plot axis
    plt.figure()
    axis = price_series.plot.bar(title=price_series.name,\
        color='tab:green', ylim=[0, 100])
    axis.set_xticklabels([ label.strftime('%b-%Y') for label in price_series.index ],\
        rotation=90, horizontalalignment='right')
    axis.get_yaxis().set_major_locator(MultipleLocator(10))
    axis.get_yaxis().set_tick_params(color='lightgray')
    axis.grid(visible=True, axis='y', which='major', color='lightgray')
    axis.spines['top'].set(visible=False)
    axis.spines['left'].set(visible=False)
    axis.spines['right'].set(visible=False)

    # Store the plot's figure and return the resulting dataset
    if save_plot:
        save_figure('fig2')
    return price_series

def rainfall_energy_price_trends(cv_rain_energy_price_df, save_plot=True):
    # Initialize and customize plot axis
    axis = cv_rain_energy_price_df.plot(title='Trends in rainfall and energy price',\
        color=['tab:blue', 'tab:green'], ylim=[0, 200])
    axis.legend(loc='upper left')
    axis.axvline(x='01/01/2017', color='darkgray', linewidth=1.5)
    axis.get_xaxis().get_label().set_visible(False)
    axis.get_yaxis().set_major_locator(MultipleLocator(20))
    axis.grid(visible=True, axis='y', which='major', color='lightgray')
    axis.set_yticklabels([ ])
    axis.get_yaxis().set_tick_params(color='white')
    axis.spines['top'].set(visible=False)
    axis.spines['left'].set(visible=False)
    axis.spines['right'].set(visible=False)

    # Store the plot's figure and return the resulting dataset
    if save_plot:
        save_figure('fig3')
    return cv_rain_energy_price_df

def rainfall_energy_price_by_year(cv_rain_energy_price_df, save_plot=True):
    # Extract data of rainfall and energy price and split each one
    # into two series, for the years 2016 and 2017, respectively.
    rainfall_series = cv_rain_energy_price_df.iloc[:, 0]
    rainfall_2016 = rainfall_series.loc[: '31/12/2016']
    rainfall_2017 = rainfall_series.loc['01/01/2017' :]
    energy_price_series = cv_rain_energy_price_df.iloc[:, 1]
    energy_price_2016 = energy_price_series.loc[: '31/12/2016']
    energy_price_2017 = energy_price_series.loc['01/01/2017' :]

    # Initialize figure and subplots and customize their axes as bar plots
    fig, axes = plt.subplots(nrows=2, ncols=2, sharey='row', figsize=(7.5, 10))
    fig.subplots_adjust(top=0.925, wspace=0, hspace=0.06)
    ((rainfall_axis_2016, rainfall_axis_2017), (energy_price_axis_2016, energy_price_axis_2017)) = axes

    # Valencian rainfall 2016
    rainfall_2016.plot.bar(ax=rainfall_axis_2016, ylim=[0, 180], color='tab:blue')
    rainfall_axis_2016.set_title(label=rainfall_2016.index[0].year, size=12, color='darkgray')
    rainfall_axis_2016.get_xaxis().set_visible(False)
    rainfall_axis_2016.set_ylabel(rainfall_series.name, size=12)
    rainfall_axis_2016.get_yaxis().set_major_locator(MultipleLocator(50))
    rainfall_axis_2016.get_yaxis().set_tick_params(color='black', labelcolor='darkgray', labelsize=8)
    rainfall_axis_2016.grid(visible=True, axis='y', which='major', color='lightgray', linewidth=0.5)
    rainfall_axis_2016.label_outer()
    rainfall_axis_2016.spines['top'].set(color='darkgray', linewidth=1.5)
    rainfall_axis_2016.spines['left'].set(color='darkgray', linewidth=1.5)
    rainfall_axis_2016.spines['right'].set(color='darkgray', linewidth=1)
    rainfall_axis_2016.spines['bottom'].set(color='lightgray', linewidth=1)
    
    # Valencian rainfall 2017
    rainfall_2017.plot.bar(ax=rainfall_axis_2017, color='tab:orange')
    rainfall_axis_2017.set_title(label=rainfall_2017.index[0].year, size=12, color='darkgray')
    rainfall_axis_2017.get_xaxis().set_visible(False)
    rainfall_axis_2017.get_yaxis().set_tick_params(color='lightgray')
    rainfall_axis_2017.grid(visible=True, axis='y', which='major', color='lightgray', linewidth=0.5)
    rainfall_axis_2017.label_outer()
    rainfall_axis_2017.spines['top'].set(color='darkgray', linewidth=1.5)
    rainfall_axis_2017.spines['right'].set(color='darkgray', linewidth=1.5)
    rainfall_axis_2017.spines['left'].set(color='darkgray', linewidth=1)
    rainfall_axis_2017.spines['bottom'].set(color='lightgray', linewidth=1)

    # Energy price 2016
    energy_price_2016.plot.bar(ax=energy_price_axis_2016, ylim=[0, 85], color='tab:blue')
    energy_price_axis_2016.get_xaxis().set_visible(False)
    energy_price_axis_2016.set_ylabel(energy_price_series.name, size=12)
    energy_price_axis_2016.get_yaxis().set_major_locator(MultipleLocator(20))
    energy_price_axis_2016.get_yaxis().set_tick_params(color='black', labelcolor='darkgray', labelsize=8)
    energy_price_axis_2016.grid(visible=True, axis='y', which='major', color='lightgray', linewidth=0.5)
    energy_price_axis_2016.label_outer()
    energy_price_axis_2016.spines['top'].set(color='lightgray', linewidth=1)
    energy_price_axis_2016.spines['right'].set(color='darkgray', linewidth=1)
    energy_price_axis_2016.spines['left'].set(color='darkgray', linewidth=1.5)
    energy_price_axis_2016.spines['bottom'].set(color='lightgray', linewidth=1.5)
    

    # Energy price 2017
    energy_price_2017.plot.bar(ax=energy_price_axis_2017, color='tab:orange')
    energy_price_axis_2017.get_xaxis().set_visible(False)
    energy_price_axis_2017.get_yaxis().set_tick_params(color='lightgray')
    energy_price_axis_2017.grid(visible=True, axis='y', which='major', color='lightgray', linewidth=0.5)
    energy_price_axis_2017.label_outer()
    energy_price_axis_2017.spines['top'].set(color='lightgray', linewidth=1)
    energy_price_axis_2017.spines['left'].set(color='darkgray', linewidth=1)
    energy_price_axis_2017.spines['right'].set(color='darkgray', linewidth=1.5)
    energy_price_axis_2017.spines['bottom'].set(color='lightgray', linewidth=1.5)

    # Store the plot's figure and return the resulting dataset
    if save_plot:
        save_figure('fig4')
    return pd.concat([
        pd.concat([rainfall_2016, rainfall_2017], axis=0),
        pd.concat([energy_price_2016, energy_price_2017], axis=0),
        ], axis=1)

def energy_demand_technological_coverage_area(energy_tech_df, save_plot=True):
    # Initialize and customize area plot axis
    axis = energy_tech_df.plot.area(title='Evolution in coverage of energy demand by technologies',\
        figsize=(18, 6), ylim=[0, 25000], colormap='Set2')
    axis.legend(loc='lower left', framealpha=0.2)
    axis.axvline(x='01/01/2017', color='k', linewidth=1.5)
    axis.get_xaxis().get_label().set_visible(False)
    axis.set_yticks(list(np.arange(0, 30000, 5000)))
    axis.set_yticklabels([ ("%dk" % (tick / 1000)) for tick in np.arange(0, 30000, 5000) ])
    axis.get_yaxis().set_tick_params(color='lightgray')
    axis.grid(visible=True, axis='y', which='major', color='lightgray')
    axis.spines['top'].set(visible=False)
    axis.spines['right'].set(visible=False)
    axis.spines['left'].set(visible=False)
    axis.get_figure().tight_layout()

    # Store the plot's figure and return the resulting dataset
    if save_plot:
        save_figure('fig5')
    return energy_tech_df

def energy_demand_technological_coverage_pie(energy_tech_df, save_plot=True):
    # Compute average energy consumption by technology
    energy_tech_avg_series = energy_tech_df.mean(axis=0)

    # Initialize and customize surface pie plot axis
    plt.figure(figsize=(8, 6))
    axis = energy_tech_avg_series.plot.pie(title='Coverage of energy demand by technologies',\
        colormap='Set2', legend=False,\
            autopct=lambda val: int(np.ceil(val / 100. * energy_tech_avg_series.sum())))
    axis.get_yaxis().get_label().set_visible(False)
    
    # Store the plot's figure and return the resulting dataset
    if save_plot:
        save_figure('fig6')
    return energy_tech_avg_series



# Load dataset, generate plots, transforming the data as needed
# and saving the chart figures generated, and display them.
if __name__ == '__main__':
    df = load_data(data_path, data_sheet_index=0)
    valencian_rainfall(df)
    energy_price(df)
    rainfall_energy_price_trends(df)
    rainfall_energy_price_by_year(df)

    df = load_data(data_path, data_sheet_index=1)
    energy_demand_technological_coverage_area(df)
    energy_demand_technological_coverage_pie(df)

    plt.show()
